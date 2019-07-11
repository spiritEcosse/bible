#include "ModulesModel.h"
#include "../tests/mock_iqsqldatabase.h"
#include "../tests/mock_iqsqlquery.h"

template <class QSqlDatabase, class QSqlQuery>
ModulesModel<QSqlDatabase, QSqlQuery>::ModulesModel(QSqlDatabase &db, QObject *parent)
    : QSqlTableModel(parent, db), db_(&db)
{
}

template <class QSqlDatabase, class QSqlQuery>
ModulesModel<QSqlDatabase, QSqlQuery>::~ModulesModel()
{
}

template <class QSqlDatabase, class QSqlQuery>
QSqlQuery& ModulesModel<QSqlDatabase, QSqlQuery>::query() const
{
    QSqlQuery q;
    return q;
}

template <class QSqlDatabase, class QSqlQuery>
void ModulesModel<QSqlDatabase, QSqlQuery>::init()
{
    setTable("modules");

    const QString s = "s";
    query_ = &query();
    query_->exec(s);

    createTable("modules", "modules_group");
    select();
}

template <class QSqlDatabase, class QSqlQuery>
bool ModulesModel<QSqlDatabase, QSqlQuery>::createTable(const QString &tableName, const QString &relatedTable)
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
        return true;
    }
    return false;
}

template <class QSqlDatabase, class QSqlQuery>
int ModulesModel<QSqlDatabase, QSqlQuery>::correctSize(const QString &str) const
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

template <class QSqlDatabase, class QSqlQuery>
QVariant ModulesModel<QSqlDatabase, QSqlQuery>::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

template <class QSqlDatabase, class QSqlQuery>
QHash<int, QByteArray> ModulesModel<QSqlDatabase, QSqlQuery>::roleNames() const {
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

template class ModulesModel<QSqlDatabase, QSqlQuery>;
template class ModulesModel<MockIQSqlDatabase, MockIQSqlQuery>;
