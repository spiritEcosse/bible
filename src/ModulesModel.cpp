#include "ModulesModel.h"
#include "../tests/mock_iqsqldatabase.h"

template <class QSqlDatabase>
ModulesModel<QSqlDatabase>::ModulesModel(QSqlDatabase &db, QObject *parent)
    : QSqlTableModel(parent, db), db_(&db)
{
}

template <class QSqlDatabase>
ModulesModel<QSqlDatabase>::~ModulesModel()
{
}

template <class QSqlDatabase>
void ModulesModel<QSqlDatabase>::init()
{
    createTable("modules", "modules_group");
    setTable("modules");
    select();
}

template <class QSqlDatabase>
bool ModulesModel<QSqlDatabase>::createTable(const QString &tableName, const QString &relatedTable)
{
    if ( !db_->tables().contains(tableName) ) {
        QString sql = QString(
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

//        db_->exec(sql);
        query();
//        if (!db_->exec(sql).exec()) {
//            qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
//        }
        return true;
    }
    return false;
}

template <class QSqlDatabase>
int ModulesModel<QSqlDatabase>::correctSize(const QString &str) const
{
    QRegularExpression re("^([+-]?\\d*\\.?\\d+)(\\w{1})*$", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(str);
    double size = 0;
    QStringList dimensions = {"K", "M", "G"};

    if (match.hasMatch()) {
        size = match.captured(1).toDouble();
        QString dimension = match.captured(2).toUpper();
        size *= qPow(1024, dimensions.indexOf(dimension) + 1);
    }
//ToDo replace on formattedDataSize
    return size;
}

template <class QSqlDatabase>
QVariant ModulesModel<QSqlDatabase>::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

template <class QSqlDatabase>
QHash<int, QByteArray> ModulesModel<QSqlDatabase>::roleNames() const {
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

template class ModulesModel<QSqlDatabase>;
template class ModulesModel<MockIQSqlDatabase>;
