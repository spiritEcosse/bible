#include <QDebug>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>

#include "userdb.h"

UserDB::UserDB()
{
    QString dbName = "user";

    if ( QSqlDatabase::contains(dbName) ) {
        db = QSqlDatabase::database(dbName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    }

    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << QString("Error: connection with %1 database failed").arg(dbName);
    } else {
        qDebug() << QString("%1 database: Connection OK").arg(dbName);
    }

    QSqlQuery query(db);

    const char* sql = "CREATE TABLE history("
                "id integer primary key, "
                "testament_index int, "
                "book_index int, "
                "chapter_index int, "
                "verse_index int)";

    if (!query.exec(QString(sql))) {
        qDebug() << query.lastError().text();
    }
}

UserDB* UserDB::getInstance()
{
    static UserDB instance;
    return &instance;
}
