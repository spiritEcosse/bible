#include "modelmodule.h"

namespace modules {

    ModelModule::ModelModule() {}

    ModelModule::~ModelModule() {}

    QVariant ModelModule::data(const QModelIndex &index, int role) const
    {
    //    if (!index.isValid() || index.row() > rowCount(index)) {
    //        return {};
    //    }

    //    const Registry& registry = m_registries.at(index.row());

    //    switch (role) {
    //        case RegistryRoles::UrlRole: {
    //            return QVariant::fromValue(registry.url());
    //        }
    //        case RegistryRoles::PriorityRole: {
    //            return QVariant::fromValue(registry.priority());
    //        }
    //        case RegistryRoles::InfoUrlRole: {
    //            return QVariant::fromValue(registry.infoUrl());
    //        }
    //        default: {
    //            return {};
    //        }
    //    }
    }

    QHash<int, QByteArray> ModelModule::roleNames() const {
        QHash<int, QByteArray> names;
        names[Qt::UserRole] = "updateDate";
        names[Qt::UserRole + 1] = "description";
        names[Qt::UserRole + 2] = "abbreviation";
        names[Qt::UserRole + 3] = "information";
        names[Qt::UserRole + 4] = "comment";
        names[Qt::UserRole + 5] = "size";
        names[Qt::UserRole + 6] = "language";
        return names;
    }

}
