#include "modulemodel.h"
#include "modelregistry.h"

namespace modules {

    ModuleModel::ModuleModel()
        : m_db { db::Db::getInstance() }
    {
    }

    ModuleModel::~ModuleModel()
    {

    }

    //void ModuleModel::init()
    //{
    ////    createTable();
    ////    setTable(tableNameString);
    ////    select();
    ////    setEditStrategy(QSqlTableModel::OnManualSubmit);
    //}

    //bool ModuleModel::execLastError(const QString& query)
    //{
    //    if (!query_->exec(query))
    //    {
    //        qWarning() << query << query_->lastError().text();
    ////        qPrintable(query_->lastError().text());
    //        return false;
    //    }
    //    return true;
    //}

    //bool ModuleModel::createTable()
    //{
    ////    if ( !database().tables().contains(tableNameString) ) {
    ////        return execLastError(qStringSql->arg(tableNameString, tableNameGroup));
    ////    }
    ////    return false;
    //}

    int ModuleModel::rowCount(const QModelIndex& parent) const
    {
        Q_UNUSED(parent)
        return static_cast<int>(m_modules.size());
    }

    QVariant ModuleModel::data(const QModelIndex &index, int role) const
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

    QHash<int, QByteArray> ModuleModel::roleNames() const {
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
