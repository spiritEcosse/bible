#include "manipulator.h"
#include "dbmapper.h"
#include <sstream>

using namespace DBTypes;

namespace db
{

Manipulator::Manipulator()
    : m_executor {new Executor {}}
{}


std::pair<DBResult, DBIndex> Manipulator::insertRow(const std::string& tableName,
                                                    const QVariantList& recordData)
{ //WARNING : using Variadic_template : https://en.wikipedia.org/wiki/Variadic_template
    const std::string& query {generateInsertQuery(tableName, recordData.size())};
    const auto& result {m_executor->execute(query, recordData)};
    return {result.first, result.second.lastInsertId().toInt()};
}

std::string Manipulator::generateBindString(size_t recordSize) const
{
    std::ostringstream bindings;
    std::fill_n(std::ostream_iterator<std::string>(bindings),
                recordSize,
                "?,");
    std::string bindingString = bindings.str();
    bindingString.pop_back();
    return bindingString;
}

std::string Manipulator::generateInsertQuery(const std::string& tableName, size_t recordSize) const
{
    std::string query {"INSERT INTO " + tableName + " (" + tablesMapping.at(tableName) + " ) " +
                  "VALUES ( "};
    query += generateBindString(recordSize);
    query += ")";
    return query;
}


}
