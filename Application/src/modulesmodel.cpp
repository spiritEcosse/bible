#include "modulesmodel.h"

ModulesModel::ModulesModel(QObject *parent)
    : QSqlTableModel(parent)
{
    init();
}

void ModulesModel::init()
{
    createTable();
    setTable(tableNameString);
    select();
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

bool ModulesModel::execLastError(const QString& query)
{
    if (!query_->exec(query))
    {
        qWarning() << query << query_->lastError().text();
//        qPrintable(query_->lastError().text());
        return false;
    }
    return true;
}

bool ModulesModel::createTable()
{
    if ( !database().tables().contains(tableNameString) ) {
        return execLastError(qStringSql->arg(tableNameString, tableNameGroup));
    }
    return false;
}

QVariant ModulesModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray> ModulesModel::roleNames() const {
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
