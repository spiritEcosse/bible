#include "Selector.h"
#include <QSqlQuery>
#include <QSqlRecord>

namespace db
{

Selector::Selector()
    : m_executor {new Executor {}}
{}

Selector::Selector(const QString& nameDb)
    : m_executor {new Executor {nameDb}}
{}


std::pair<DBResult, std::vector<DBEntry>> Selector::selectAll(const QString &tableName)
{
    QString query {generateQuery(tableName)};
    DBResult result;
    QSqlQuery resultQuery;
    std::tie(result, resultQuery) = m_executor->execute(query);

    std::vector<DBEntry> returnData;
    if (result == DBResult::OK)
    {
//        returnData.clear();
//        returnData.reserve(resultQuery.size());
        while (resultQuery.next())
        {
            const QSqlRecord& entryRecord {resultQuery.record()};
            QVariantList entryData;
            entryData.reserve(entryRecord.count());

            for (int i = 0; i < entryRecord.count(); ++i)
            {
                entryData.push_back(entryRecord.value(i));
            }

            returnData.push_back(std::move(entryData));
        }
    }

    return {result, returnData};
}

QString Selector::generateQuery(const QString &tableName) const
{
    QString query {"SELECT * FROM " + tableName};
    return query;
}

}
