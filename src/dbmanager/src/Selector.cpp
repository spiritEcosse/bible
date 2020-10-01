#include "Selector.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include "Executor.h"
#include "registry.h"

using namespace DBTypes;

namespace db
{

template<class T>
Selector<T>::Selector()
    : m_executor {new Executor<T> {}}
{}

template<class T>
Selector<T>::Selector(const QString& nameDb)
    : m_executor {new Executor<T> {nameDb}}
{
}

template<class T>
DBResult Selector<T>::selectAll(const std::string& tableName, std::vector<QVariantList>& returnData)
{
    const std::string query {generateQuery(tableName)};
    DBResult result;
    QSqlQuery resultQuery;
    std::tie(result, resultQuery) = m_executor->execute(query);

    if (result == DBResult::OK)
    {
        while (resultQuery.next())
        {
            const QSqlRecord& resultRecord = resultQuery.record();
            QVariantList result;
            for (int i = 0; i < resultRecord.count(); ++i)
            {
                result.push_back(resultRecord.value(i));
            }
            returnData.push_back(std::move(result));
        }
    }

    return result;
}

template<class T>
std::string Selector<T>::generateQuery(const std::string& tableName) const
{
    std::string query = "SELECT rowid, * FROM " + tableName;
    return query;
}

}

template class db::Selector<Registry>;
