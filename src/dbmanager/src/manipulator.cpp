#include "manipulator.h"
#include "dbmapper.h"
#include <sstream>
#include "registry.h"

using namespace DBTypes;


namespace db
{

template<class T>
Manipulator<T>::Manipulator()
    : m_executor {new Executor {}}
{}


template<class T>
std::pair<DBResult, DBIndex> Manipulator<T>::insertRow(const std::string& tableName,
                                                    const QVariantList& recordData)
{ //WARNING : using Variadic_template : https://en.wikipedia.org/wiki/Variadic_template
    const std::string& query {generateInsertQuery(tableName, recordData.size())};
    const auto& result {m_executor->execute(query, recordData)};
    return {result.first, result.second.lastInsertId().toInt()};
}

template<class T>
void Manipulator<T>::insertBulk(const T &container)
{
}

template<class T>
std::string Manipulator<T>::generateBindString(size_t recordSize) const
{
    std::ostringstream bindings;
    std::fill_n(std::ostream_iterator<std::string>(bindings),
                recordSize,
                "?,");
    std::string bindingString = bindings.str();
    bindingString.pop_back();
    return bindingString;
}

template<class T>
std::string Manipulator<T>::generateInsertQuery(const std::string& tableName, size_t recordSize) const
{
    std::string query {"INSERT INTO " + tableName + " (" + tablesMapping.at(tableName) + " ) " +
                  "VALUES ( "};
    query += generateBindString(recordSize);
    query += ")";
    return query;
}


}

template class db::Manipulator<Registry>;
