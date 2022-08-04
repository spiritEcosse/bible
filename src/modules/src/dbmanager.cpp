#include <QDebug>
#include <QString>

#include "dbmanager.h"

DbManager::DbManager(const QString& moduleName)
{
//    QString langCode(getenv("LANG"));
//    if (langCode.isEmpty() || langCode == "C" || !langCode.contains("_"))
//        langCode = QLocale::system().name();
//    if (langCode.contains('.'))
//        langCode = langCode.mid(0, langCode.lastIndexOf('.'));
//    if (langCode.contains("_"))
//        langCode = langCode.mid(0, langCode.lastIndexOf('_'));
//    if (langCode == "C")
//        langCode = "ru";

    QString db_name(QDir::currentPath() + "/modules/" + moduleName + "/.SQLite3");

//    qDebug() << "Current language code is" << langCode << db_name;

    if ( QSqlDatabase::contains(db_name) ) {
        db = QSqlDatabase::database(db_name);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", db_name);
    }
    db.setDatabaseName(db_name);

    if (!db.open()) {
        qDebug() << "Error: connection with Bible database failed";
    } else {
        qDebug() << "Bible database: Connection OK";
    }
}


DbManagerComments::DbManagerComments(const QString& moduleName)
{
    QString dbNameComments(QDir::currentPath() + "/modules/" + moduleName + "/.commentaries.SQLite3");

    if ( QSqlDatabase::contains(dbNameComments) ) {
        db = QSqlDatabase::database(dbNameComments);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", dbNameComments);
    }
    db.setDatabaseName(dbNameComments);

    if (!db.open()) {
        qDebug() << "Error: connection with Bible database failed";
    } else {
        qDebug() << "Bible database: Connection OK";
    }
}
