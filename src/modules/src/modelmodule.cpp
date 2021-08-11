#include "modelmodule.h"
#include <QObject>
#include <QDebug>
#include <QtQuick>
#include <QJsonDocument>
#include <QJsonArray>

namespace modules {

    using namespace sqlite_orm;

    inline auto statementExtraFields() {
        std::unique_ptr<db::Db<Module>> m_db { new db::Db<Module>() };
        return m_db->storage->prepare(
                    select(
                        columns(
                            &Module::m_selected,
                            &Module::m_downloaded,
                            &Module::m_id,
                            &Module::m_idGroupModules,
                            &Module::m_abbreviation
                        ),
                    where(c(&Module::m_selected) == true or c(&Module::m_downloaded) == true)
               )
        );
    }

    ModelModule::ModelModule(int idGroupModules, const QString& needle)
        : m_idGroupModules (idGroupModules),
          m_needle (std::move(needle))
    {
        connect(this, &ModelModule::updateDone, this, &ModelModule::saveExtraFieldsToDb);
        updateObjects();
    }

    void ModelModule::registerMe()
    {
        qmlRegisterType<ModelModule>("bible.ModelModule", 1, 0, "ModelModule");
    }

    ModelModule::ModelModule() {}

    ModelModule::~ModelModule() {}

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

    void ModelModule::getExtraFieldsFromDb()
    {
        m_extraFields = {};

        for(const auto &row : m_db->storage->execute(statementExtraFields())) {
            m_extraFields.push_back(std::tuple(std::get<4>(row), std::get<0>(row), std::get<1>(row)));
        }
    }

    void ModelModule::saveExtraFieldsToDb()
    {

    }

    int ModelModule::countActive()
    {
        return m_db->storage->count<Module>(where(c(&Module::m_hidden) == false));
    }

    void ModelModule::updateSelected(int id, bool selected) const
    {
        m_db->storage->update_all(
                    set(assign(&Module::m_selected, selected)),
                    where(c(&Module::m_id) == id));
    }

    void ModelModule::updateSelectedBulk(const QVariantList& data) const
    {
        std::vector<int> ids;
        for (const auto &obj : data) {
            ids.push_back(obj.toMap()["moduleId"].toInt());
        }
        m_db->storage->update_all(
                    set(assign(&Module::m_selected, false)),
                    where(in(&Module::m_id, ids)));
    }

    void ModelModule::updateDownloaded(int id, bool downloaded) const
    {
        m_db->storage->update_all(
                    set(assign(&Module::m_downloaded, downloaded)),
                    where(c(&Module::m_id) == id));
    }

    QVariant ModelModule::getExtraFields()
    {
        const auto &data = m_db->storage->execute(statementExtraFields());

        QJsonArray selectedArray;
        QJsonArray downloadedArray;

        for(const auto &row : data) {
            if (bool selected = std::get<0>(row))
            {
                selectedArray << QJsonObject { {"selecting", selected }, { "moduleId", std::get<2>(row) }, { "groupId", std::get<3>(row) } };
            }

            if (bool downloaded = std::get<1>(row))
            {
                downloadedArray << QJsonObject { {"downloaded", downloaded }, { "moduleId", std::get<2>(row) }, { "groupId", std::get<3>(row) } };
            }
        }

        QJsonObject document {
            {"selected", std::move(selectedArray)},
            {"downloaded", std::move(downloadedArray)}
        };
        return std::move(document);
    }

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
        };
    }

}
