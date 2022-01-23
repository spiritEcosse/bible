#include <QDebug>
#include <QString>

#include "dbmanager.h"

DbManager::DbManager(QString&& fileName)
{
    QString langCode(getenv("LANG"));
    if (langCode.isEmpty() || langCode == "C" || !langCode.contains("_"))
        langCode = QLocale::system().name();
    if (langCode.contains('.'))
        langCode = langCode.mid(0, langCode.lastIndexOf('.'));
    if (langCode.contains("_"))
        langCode = langCode.mid(0, langCode.lastIndexOf('_'));
    if (langCode == "C")
        langCode = "ru";

    QString db_name = std::move(fileName);
    qDebug() << "Current language code is" << langCode << db_name;

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

//    db_name = "/usr/share/bible/db/.commentaries.SQLite3";

//    if ( QSqlDatabase::contains(db_name) ) {
//        db_comments = QSqlDatabase::database(db_name);
//    } else {
//        db_comments = QSqlDatabase::addDatabase("QSQLITE", db_name);
//    }
//    db_comments.setDatabaseName(db_name);

//    if (!db_comments.open()) {
//        qDebug() << "Error: connection with Bible database failed";
//    } else {
//        qDebug() << "Bible database: Connection OK";
//    }
}


