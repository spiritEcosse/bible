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
std::pair<DBResult, QSqlQuery> Executor<T>::
executeB(const std::string &queryText, const std::vector<T> &container)
{
    if (!m_connectionManager->isValid()) {
        qCritical() << "Database is not valid, skip!";
        return {DBResult::FAIL, QSqlQuery{}};
    }

    QSqlQuery query {QString::fromStdString(queryText)};
    QVariantList urls;
    QVariantList priority;
    QVariantList infoUrl;

    for (auto it = container.begin(); it != container.end(); ++it) {
        urls << it->url();
        priority << it->priority();
        infoUrl << it->infoUrl();
    }

    query.addBindValue(urls);
    query.addBindValue(priority);
    query.addBindValue(infoUrl);

    DBResult result {DBResult::OK};

    if (!query.execBatch() && query.lastError().isValid())
    {
        qWarning() << bool(result);
        qCritical() << query.lastError().text() << query.lastQuery();
        result = DBResult::FAIL;
    }
    return {result, query};
};

}

template class db::Executor<Registry>;
