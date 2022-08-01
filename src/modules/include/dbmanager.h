#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

class DbManager
{
public:
    DbManager(const QString& moduleName);
    QSqlDatabase db;
};


class DbManagerComments
{
public:
    DbManagerComments(const QString& moduleName);
    QSqlDatabase db;
};

#endif // DBMANAGER_H
