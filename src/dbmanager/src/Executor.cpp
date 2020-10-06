#include "Executor.h"
#include "ConnectionManager.h"
#include <QDebug>
#include <QSqlError>
#include <QString>

#include "registry.h"

using namespace DBTypes;

namespace db
{

template<class T>
Executor<T>::Executor()
    : m_connectionManager {new ConnectionManager {}}
{}

template<class T>
Executor<T>::Executor(const QString& nameDb)
    : m_connectionManager {new ConnectionManager {nameDb}} {}

template<class T>
std::pair<DBResult, QSqlQuery> Executor<T>::execute(const std::string& queryText, const QVariantList &args)
{
    if (!m_connectionManager->isValid()) {
        qCritical() << "Database is not valid, skip!";
        return {DBResult::FAIL, QSqlQuery{}};
    }
    QSqlQuery query {QString::fromStdString(queryText)};

    for (int i = 0; i < args.size(); ++i)
    {
        query.bindValue(i, args[i]);
    }

    DBResult result {DBResult::OK};

    if (!query.exec() && query.lastError().isValid())
    {
        qCritical() << query.lastError().text() << query.lastQuery();
        result = DBResult::FAIL;
    }
    return {result, query};
}

template<class T>
void Executor<T>::executeBatch(const std::string &queryText, std::vector<T>& container)
{
    if (!m_connectionManager->isValid()) {
        qCritical() << "Database is not valid, skip!";
//        return {DBResult::FAIL, QSqlQuery{}};
    }

    QSqlQuery query {QString::fromStdString(queryText)};
    const auto& columns = T::getColumns();

    for (auto itColumn = columns.begin(); itColumn != columns.end(); itColumn++) {
        QVariantList data;

        for (auto it = container.begin(); it != container.end(); it++) {
            data << QMetaObject::invokeMethod(&*it, *itColumn, Qt::DirectConnection);
        }
        query.addBindValue(data);
    }

    if (!query.execBatch() && query.lastError().isValid())
    {
        qCritical() << query.lastError().text() << query.lastQuery();
    }
};

}

template class db::Executor<Registry>;
