#include "Executor.h"
#include "ConnectionManager.h"
#include <QDebug>
#include <QSqlError>
#include <QString>

#include <cpp_redis/cpp_redis>
#include <tacopie/tacopie>

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
    cpp_redis::client client;

//    client.connect(std::string("192.168.1.131"));

    client.connect("192.168.1.131", 6379, [](const std::string& host, std::size_t port, cpp_redis::connect_state status) {
      if (status == cpp_redis::connect_state::dropped) {
        std::cout << "client disconnected from " << host << ":" << port << std::endl;
      }
    });

    for (int i = 0; i < 500; i++) {
        client.set("hello", std::to_string(i));
    }

    client.commit();  // for asynchronous call

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
