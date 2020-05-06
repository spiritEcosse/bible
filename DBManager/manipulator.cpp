#include "manipulator.h"
#include "dbmapper.h"
#include <sstream>

namespace db
{

Manipulator::Manipulator()
    : m_executor {new Executor {}}
{}

Manipulator::Manipulator(const QString& nameDb)
    : m_executor {new Executor {nameDb}}

{}

std::pair<DBResult, DBIndex> Manipulator::insertRow(const QString &tableName,
                                                    const QVariantList &recordData)
{ //WARNING : using Variadic_template : https://en.wikipedia.org/wiki/Variadic_template
    const QString& query {generateInsertQuery(tableName, recordData.size())};
    const auto& result {m_executor->execute(query, recordData)};
    return {result.first, result.second.lastInsertId().toInt()};
}

QString Manipulator::generateBindString(size_t recordSize) const
{
    std::ostringstream bindings;
    std::fill_n(std::ostream_iterator<std::string>(bindings),
                recordSize,
                "?,");
    std::string bindingString = bindings.str();
    bindingString.pop_back();
    return QString::fromStdString(bindingString);
}

QString Manipulator::generateInsertQuery(const QString &tableName, size_t recordSize) const
{
    QString query {"INSERT INTO " + tableName + " (" + TableMapping.at(tableName) + " ) " +
                  "VALUES ( "};
    query += generateBindString(recordSize);
    query += ")";
    return query;
}


}
