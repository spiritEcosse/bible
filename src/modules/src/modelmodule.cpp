#include "modelmodule.h"
#include <QObject>
#include <QDebug>
#include <QtQuick>
#include <QJsonDocument>
#include <QJsonArray>
#include <JlCompress.h>

namespace modules {

    static std::once_flag flagInit;

    Worker::Worker(QObject *parent)
        : QObject(parent),
          m_modelHost { new ModelHost {} }
    {
    }

    Worker::~Worker() {}

    // helpers
    bool Worker::bulkUpdateDownloaded(const Downloaded &downloaded, bool value)
    {
        updateAllIn(&Module::m_downloaded, value, &Module::m_name, downloaded);
        return true;
    }

    // delete
    void Worker::deleteFiles(const Downloaded &downloaded)
    {
        std::for_each(downloaded.begin(), downloaded.end(), [](const auto& module) {
            QDir dir (QDir::currentPath() + "/modules/" + std::get<0>(module));
            dir.removeRecursively();
        });
        bulkUpdateDownloaded(downloaded, false);
        emit deleteCompleted();
    }

    // download
    bool Worker::startDownloadModules(const Downloaded &downloaded)
    {
        m_multi.reset(new CurlMulti(makeUrls(downloaded)));
        connect(m_multi.get(), &CurlMulti::downloaded, this, &Worker::updateSuccessfullyDownloaded);
        connect(m_multi.get(), &CurlMulti::downloaded, this, &Worker::retryFailedDownloaded);
        QTimer::singleShot(0, m_multi.get(), &CurlMulti::perform);
        return true;
    }

    std::vector<QString> Worker::makeUrls(const Downloaded& downloaded) const
    {
        std::vector<QString> urls;
        std::transform(downloaded.begin(), downloaded.end(), std::back_inserter(urls),
                       [this](const auto& moduleName)
        {
            return std::move(QString().sprintf(std::move(m_modelHost->getUrl(index).toLocal8Bit().data()),
                                               std::move(std::get<0>(moduleName).toLocal8Bit().data())));
        });
        return urls;
    }

    bool Worker::unarchiveModules(const Downloaded &downloaded)
    {
        const QString dir = QDir::currentPath() + "/modules/";
        std::for_each(downloaded.begin(), downloaded.end(), [&dir](const auto& module) {
            const QString& moduleName = std::get<0>(module);
            JlCompress::extractDir(dir + moduleName + ".zip", dir + "/" + moduleName);
            QFile::remove(dir + moduleName + ".zip");
        });
        return true;
    }

    void Worker::updateSuccessfullyDownloaded()
    {
        const Downloaded& downloaded = m_multi->getSuccessfullyDownloaded();
        !downloaded.empty() && bulkUpdateDownloaded(downloaded) && unarchiveModules(downloaded);
    }

    void Worker::retryFailedDownloaded()
    {
        const Downloaded &downloaded = m_multi->getFailedDownloaded();
        (!downloaded.empty() && m_modelHost->existsIndex(++index) && startDownloadModules(downloaded)) ||
                emit downloadCompleted();
    }

    // ModelModule
    ModelModule::ModelModule(int idGroupModules, const QString& needle)
        : m_idGroupModules (idGroupModules),
          m_needle (std::move(needle))
    {
        connect(this, &ModelModule::updateDone, this, &ModelModule::saveExtraFieldsToDb);
        updateObjects();
    }

    ModelModule::ModelModule(QObject *parent)
        : ModelUpdate(parent),
          m_worker { new Worker() }
    {
        qRegisterMetaType<Downloaded>("Downloaded");
        m_worker->moveToThread(&workerThread);
//        connect(&workerThread, &QThread::finished, m_worker.get(), &QObject::deleteLater);
        connect(this, &ModelModule::startDeleteFiles, m_worker.get(), &Worker::deleteFiles);
        connect(this, &ModelModule::startDownloadModules, m_worker.get(), &Worker::startDownloadModules);
        connect(m_worker.get(), &Worker::deleteCompleted, this, &ModelModule::postDeleteFiles);
        connect(m_worker.get(), &Worker::downloadCompleted, this, &ModelModule::postDownloaded);
        workerThread.start();
    }

    ModelModule::~ModelModule()
    {
        workerThread.quit();
        workerThread.wait();
    }

    // helpers
    void ModelModule::init()
    {
        std::call_once(flagInit, [this]() {
            retrieveDownloaded();
            retrieveSelecteded();
        });
    }

    void ModelModule::registerMe()
    {
        qmlRegisterType<ModelModule>("bible.ModelModule", 1, 0, "ModelModule");
    }

    Downloaded ModelModule::transformFromQVariant(const QVariantList& downloaded) const
    {
        Downloaded data;
        std::transform(downloaded.begin(), downloaded.end(), std::back_inserter(data),
                       [](const auto& obj)
        {
            return std::make_tuple(obj.toMap()["moduleId"].toString());
        });
        return data;
    }

    const QString ModelModule::getNameJson()
    {
        return QString("downloads");
    }

    // db queries get
    int ModelModule::countActive()
    {
        return m_db->storage->count<Module>(where(c(&Module::m_hidden) == false));
    }

    void ModelModule::updateObjects()
    {
        if (m_needle.isEmpty())
        {
            beginResetModel();
            objectsCount = 0;

            m_objects = m_db->storage->get_all_pointer<Module>(
                    where(c(&Module::m_idGroupModules) == m_idGroupModules),
                    multi_order_by(
                            order_by(&Module::m_hidden),
                            order_by(&Module::m_abbreviation)
                            ));
            endResetModel();
        } else {
            search();
        }
    }

    void ModelModule::search()
    {
        beginResetModel();
        objectsCount = 0;

        m_objects = m_db->storage->get_all_pointer<Module>(
                where(
                        c(&Module::m_idGroupModules) == m_idGroupModules and
                        like(&Module::m_abbreviation, m_needle + "%")
                ),
                multi_order_by(
                        order_by(&Module::m_hidden),
                        order_by(&Module::m_abbreviation)
                ));
        endResetModel();
    }

    void ModelModule::updateObjectsDownloaded()
    {
        beginResetModel();
        objectsCount = 0;

        m_objects = m_db->storage->get_all_pointer<Module>(
                inner_join<GroupModules>(on(c(&GroupModules::m_groupId) == &Module::m_idGroupModules)),
                where(c(&Module::m_downloaded) == true and
                      c(&GroupModules::m_name) == "Translations"),
                order_by(&Module::m_abbreviation)
                    );
        endResetModel();
    }

    void ModelModule::updateObjectsActive()
    {
        beginResetModel();
        objectsCount = 0;

        m_objects = m_db->storage->get_all_pointer<Module>(
                where(c(&Module::m_active) == true)
                    );
        endResetModel();
    }

    // db queries update
    void ModelModule::updateSelected(int id, bool selected) const
    {
        updateAllC(&Module::m_selected, selected, &Module::m_id, id);
    }

    void ModelModule::activateModule(int id) const
    {
        updateAllC(&Module::m_active, false, &Module::m_active, true);
        updateAllC(&Module::m_active, true, &Module::m_id, id);
    }

    void ModelModule::updateSelectedBulk(const QVariantList& data) const
    {
        std::vector<int> ids;
        for (const auto &obj : data) {
            ids.push_back(obj.toMap()["moduleId"].toInt());
        }
        updateAllIn(&Module::m_selected, false, &Module::m_id, ids);
    }

    void ModelModule::updateDownloaded(int id, bool downloaded) const
    {
        updateAllC(&Module::m_downloaded, downloaded, &Module::m_id, id);
    }

    bool ModelModule::bulkUpdateDownloaded(const Downloaded &downloaded, bool value)
    {
        updateAllIn(&Module::m_downloaded, value, &Module::m_name, downloaded);
        return true;
    }

    // getters
    QVariantList ModelModule::getDownloaded()
    {
        return m_downloaded;
    }

    QVariantList ModelModule::getSelected()
    {
        return m_selected;
    }

    bool ModelModule::getDeleteCompleted()
    {
        return m_deleteCompleted;
    }

    bool ModelModule::getDownloadCompleted()
    {
        return m_downloadCompleted;
    }

    // setters
    bool ModelModule::setDeleteCompleted(bool value)
    {
        m_deleteCompleted = value;
        emit changeDeleteCompleted();
        return true;
    }

    bool ModelModule::setDownloadCompleted(bool value)
    {
        m_downloadCompleted = value;
        emit changeDownloadCompleted();
        return true;
    }

    void ModelModule::retrieveDownloaded()
    {
        m_downloaded.clear();
        for(const auto &row: getDataByFieldTrue(&Module::m_downloaded, &Module::m_id,
                                                &Module::m_idGroupModules))
        {
            m_downloaded << QJsonObject {
                { "moduleId", std::get<0>(row) },
                { "groupId", std::get<1>(row) }
            };
        }
        emit changeDownloaded();
    }

    void ModelModule::retrieveSelecteded()
    {
        m_selected.clear();
        for(const auto &row: getDataByFieldTrue(&Module::m_selected, &Module::m_id,
                                                &Module::m_idGroupModules))
        {
            m_selected << QJsonObject {
                { "moduleId", std::get<0>(row) },
                { "groupId", std::get<1>(row) }
            };
        }
        emit changeSelected();
    }

    void ModelModule::getExtraFieldsFromDb()
    {
        m_selectedBackup = getDataByFieldTrue(&Module::m_selected, &Module::m_name);
        m_downloadedBackup = getDataByFieldTrue(&Module::m_downloaded, &Module::m_name);
    }

    void ModelModule::saveExtraFieldsToDb()
    {
        bulkUpdateDownloaded(m_downloadedBackup);
        m_downloadedBackup.clear();
        updateAllIn(&Module::m_selected, true, &Module::m_name, m_selectedBackup);
        m_selectedBackup.clear();
    }

    // download
    void ModelModule::downloadModules(const QVariantList& downloaded)
    {
        const auto &data = getActualDataByField(&Module::m_id, transformFromQVariant(downloaded), &Module::m_downloaded, false, &Module::m_name);
        !data.empty() && setDownloadCompleted(false) && emit startDownloadModules(data);
    }

    void ModelModule::postDownloaded()
    {
        setDownloadCompleted(true);
        retrieveDownloaded();
    }

    // delete
    void ModelModule::deleteModules(const QVariantList &downloaded)
    {
        const auto &data = getActualDataByField(&Module::m_id, transformFromQVariant(downloaded),  &Module::m_downloaded, true, &Module::m_name);
        !data.empty() && setDeleteCompleted(false) && emit startDeleteFiles(data);
    }

    void ModelModule::postDeleteFiles()
    {
        setDeleteCompleted(true);
        retrieveDownloaded();
    }

    // overriden from qt
    QVariant ModelModule
    ::data(const QModelIndex & index, int role) const {
        QVariant data {};

        if (!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &modules = m_objects.at(index.row());
        QString str;

        switch(role) {
            case Name :
                data = std::move(modules->m_name);
                break;
            case DateUpdate :
                data = std::move(modules->m_update);
                break;
            case Size :
                data = std::move(modules->m_size);
                break;
            case Description :
                data = std::move(modules->m_description);
                break;
            case AdditionalInfo :
                str = std::move(modules->m_information);
                str += std::move(modules->m_copyright);
                str += std::move(modules->m_comment);
                data = std::move(str);
                break;
            case Abbreviation :
                data = std::move(modules->m_abbreviation);
                break;
            case Hidden :
                data = std::move(modules->m_hidden);
                break;
            case Id :
                data = std::move(modules->m_id);
                break;
            case Downloading :
                data = false;
                break;
            case Book :
                qDebug() << "books";
                if (modules->m_books == nullptr) {
                    QString fileName (QDir::currentPath() + "/modules/" + modules->m_name + "/.SQLite3");
                    DbManager db (std::move(fileName));
                    modules->m_books.reset(new BooksModel(db.db));
                }
                data = qVariantFromValue(modules->m_books.get());
                break;
        }

        return data;
    }

    QHash<int, QByteArray>
    ModelModule::roleNames() const {
        return {
            { Name, "name" },
            { Description, "description" },
            { Abbreviation, "abbreviation" },
            { Size, "size" },
            { LanguageShow, "language_show" },
            { AdditionalInfo, "additional_info" },
            { DateUpdate, "date" },
            { Hidden, "hid" },
            { DefaultDownload, "defaultDownload" },
            { Id, "id" },
            { Downloading, "downloading" },
            { Book, "books" },
        };
    }

}
