#include <QtQuick>
#include "modelmoduledownload.h"
#include "module.h"

namespace modules {

    using namespace sqlite_orm;

    ModelModuleDownload::ModelModuleDownload()
    {
        updateObjects();
    }

    ModelModuleDownload::~ModelModuleDownload() {}

    void ModelModuleDownload::registerMe()
    {
        qmlRegisterType<ModelModuleDownload>("bible.ModelModuleDownload", 1, 0, "ModelModuleDownload");
    }

    void ModelModuleDownload::clearObjects()
    {
        m_objects.clear();
        objectsCount = 0;
    }

    void ModelModuleDownload::updateObjects()
    {
        beginResetModel();
        const auto &data = m_db->storage->select(
                    columns(
                            &ModuleDownload::m_selecting,
                            &ModuleDownload::m_downloaded,
                            &Module::m_id,
                            &Module::m_idGroupModules,
                            &ModuleDownload::m_id),
                    inner_join<Module>(on(c(&ModuleDownload::m_abbreviation) == &Module::m_abbreviation))
                    );
        clearObjects();

        for(const auto &row : data) {
            ModuleDownloadUnique moduleDownload = std::make_unique<ModuleDownload>(
                        "", std::get<0>(row), std::get<1>(row), std::get<2>(row), std::get<3>(row), std::get<4>(row)
                        );
            m_objects.push_back(std::move(moduleDownload));
        }
        objectsCount = m_objects.size();
        endResetModel();
    }

    void ModelModuleDownload::crudWithSelecting(const QString& abbreviation, bool selecting)
    {
        const auto &objects = m_db->storage->get_all_pointer<ModuleDownload>(
                    where(c(&ModuleDownload::m_abbreviation) == abbreviation));
        if (objects.size()) {
            auto &object = objects[0];
            object->m_selecting = selecting;
            !object->m_downloaded && !selecting ? remove(object->m_id) : m_db->storage->update(*object);
        } else {
            create(abbreviation, selecting, false);
        }
    }

    void ModelModuleDownload::crudWithDownloaded(const QString& abbreviation, bool downloaded)
    {
        const auto &objects = m_db->storage->get_all_pointer<ModuleDownload>(
                    where(c(&ModuleDownload::m_abbreviation) == abbreviation));
        if (objects.size()) {
            auto &object = objects[0];
            object->m_downloaded = downloaded;
            !object->m_selecting && !downloaded ? remove(object->m_id) : m_db->storage->update(*object);
        } else {
            create(abbreviation, false, downloaded);
        }
    }

    void ModelModuleDownload::create(const QString& abbreviation, bool selecting, bool downloaded)
    {
        ModuleDownloadUnique object = std::make_unique<ModuleDownload>(abbreviation, selecting, downloaded);
        m_db->storage->insert(*object);
    }

    void ModelModuleDownload::remove(int id)
    {
        m_db->storage->remove<ModuleDownload>(id);
    }

    QVariant ModelModuleDownload
    ::data(const QModelIndex & index, int role) const {
        QVariant data {};

        if (!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &object = m_objects.at(index.row());

        switch(role) {
            case ModuleIdRole :
                data = std::move(object->m_moduleId);
                break;
            case GroupIdRole :
                data = std::move(object->m_groupId);
                break;
            case SelectingRole :
                data = std::move(object->m_selecting);
                break;
            case DownloadedRole :
                data = std::move(object->m_downloaded);
                break;
            case DownloadedIdRole :
                data = std::move(object->m_id);
                break;
        }

        return data;
    }

    QHash<int, QByteArray>
    ModelModuleDownload::roleNames() const {
        return {
            { ModuleIdRole, "module_id" },
            { GroupIdRole, "group_id" },
            { SelectingRole, "selecting" },
            { DownloadedRole, "downloaded" },
            { DownloadedIdRole, "id" },
        };
    }

}
