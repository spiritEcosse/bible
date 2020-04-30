#include "LocaleDesc.h"

static const char *tableName = "locale_desc";

//static void createTable()
//{
//    if (!QSqlDatabase::database().tables().contains(tableName)) {
//        QSqlQuery query;
//        QString sql;

//        sql = QString(
//                    "CREATE TABLE IF NOT EXISTS '%1' ("
//                    "   'id'            INTEGER PRIMARY KEY AUTOINCREMENT, "
//                    "   'language'      CHAR(50) NOT NULL, "
//                    "   'description'   TEXT NOT NULL, "
//                    "   '%2_id'         NUMERIC NOT NULL, "
//                    "FOREIGN KEY ('%2_id') REFERENCES %2(id)"
//                    ")"
//                    ).arg(tableName, 'modules');

//        if (!query.exec(sql)) {
//            qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
//        }
//    }
//}

//LocaleDesc::LocaleDesc()
//{

//}
