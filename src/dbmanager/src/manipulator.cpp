#include "manipulator.h"
#include "dbmapper.h"
#include <sstream>
#include <QDebug>
#include "registry.h"

using namespace DBTypes;


namespace db
{

template<class T>
Manipulator<T>::Manipulator()
    : m_executor {new Executor<T> {}}
{}


template<class T>
std::pair<DBResult, DBIndex> Manipulator<T>::insertRow(const std::string& tableName,
                                                    const QVariantList& recordData)
{ //WARNING : using Variadic_template : https://en.wikipedia.org/wiki/Variadic_template
//    const std::string& query {generateInsertQuery(tableName, recordData.size())};
//    const auto& result {m_executor->execute(query, recordData)};
//    return {result.first, result.second.lastInsertId().toInt()};
}

template<class T>
void Manipulator<T>::insertBulk(const std::vector<T>& container)
{
    const QString& query {generateInsertQuery(container.size())};
    qWarning() << query;
//    qWarning() << T::className();
//    for (auto it = container.begin(); it != container.end(); ++it) {
//        qWarning() << *it;
//    }
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
QString Manipulator<T>::generateInsertQuery(size_t recordSize) const
{
    QString query = QString("INSERT INTO %1 (%2) values ();").arg(T::tableName(), T::getColumns());
//    query += generateBindString(recordSize);
    return query;
}


}

template class db::Manipulator<Registry>;
