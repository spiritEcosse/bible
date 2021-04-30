#include "modelmodule.h"
#include <QObject>

namespace modules {

    using namespace sqlite_orm;

    ModelModule::ModelModule(int idGroupModules)
        : m_idGroupModules (idGroupModules)
    {
        updateObjects();
    }

    ModelModule::ModelModule() {}

    ModelModule::~ModelModule() {}

    void ModelModule::updateObjects()
    {
        beginResetModel();
        objectsCount = 0;
        m_objects = m_db->storage->get_all_pointer<Module>(
                    where(c(&Module::m_idGroupModules) == m_idGroupModules),
                    order_by(&Module::m_abbreviation));
        endResetModel();
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
                str = std::move(modules->m_description);
                str += std::move(modules->m_information);
                str += std::move(modules->m_copyright);
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
            { Information, "information" },
            { Comment, "comment" },
            { Copyright, "copyright" },
            { DateUpdate, "date" },
            { Hidden, "hid" },
            { DefaultDownload, "defaultDownload" },
            { Id, "id" },
        };
    }

}
