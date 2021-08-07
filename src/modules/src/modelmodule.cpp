#include "modelmodule.h"
#include <QObject>
#include <QDebug>
#include <QtQuick>

namespace modules {

    using namespace sqlite_orm;

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
                    order_by(&Module::m_abbreviation));
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
                order_by(
                    order_by(&Module::m_abbreviation)
                ));
        endResetModel();
    }

    void ModelModule::saveExtraFieldsFromDb()
    {

    }

    void ModelModule::saveExtraFieldsToDb()
    {

    }

    int ModelModule::countAll()
    {
        return m_db->storage->count<Module>();
    }

    void ModelModule::updateSelecting(int id, bool selecting) const
    {
        const auto &object = m_db->storage->get_pointer<Module>(id);
        object->m_selecting = selecting;
        m_db->storage->update(*object);
    }

    void ModelModule::updateDownloaded(int id, bool downloaded) const
    {
        const auto &object = m_db->storage->get_pointer<Module>(id);
        object->m_downloaded = downloaded;
        m_db->storage->update(*object);
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
