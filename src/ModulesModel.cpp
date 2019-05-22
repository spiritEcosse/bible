#include "ModulesModel.h"

static void createTable(QString const tableName, QString const relatedTable)
{
    if (!QSqlDatabase::database().tables().contains(tableName)) {
        QSqlQuery query;
        QString sql;

        sql = QString(
                    "CREATE TABLE IF NOT EXISTS '%1' ("
                    "   'id'                INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "   'name'              CHAR(200) NOT NULL, "
                    "   'description'       TEXT, "
                    "   'abbreviation'      CHAR(50), "
                    "   'information'       TEXT, "
                    "   'language'          CHAR(50), "
                    "   'language_show'     CHAR(50), "
                    "   'update'            TEXT, "
                    "   'urls'              TEXT, "
                    "   'comment'           TEXT, "
                    "   'size'              NUMERIC NOT NULL, "
                    "   'region'            TEXT, "
                    "   'default_download'  NUMERIC DEFAULT 0, "
                    "   'hidden'            NUMERIC DEFAULT 0, "
                    "   'copyright'         TEXT, "
                    "   '%2_id'             NUMERIC NOT NULL, "
                    "FOREIGN KEY ('%2_id')  REFERENCES %2(id)"
                ")"
                    ).arg(tableName, relatedTable);

        if (!query.exec(sql)) {
            qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
        }
    }
}

ModulesModel::ModulesModel(QObject *parent)
    : QSqlTableModel(parent)
{
    createTable(tableName(), "modules_group");
    setTable(tableName());
    select();
}

QString ModulesModel::tableName() const
{
    return "modules";
}

ModulesModel::~ModulesModel()
{
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
