#pragma once
#include "dbtypes.h"
#include <vector>
#include <QVariantList>
#include <memory>

namespace db
{
template<class T>
class Executor;

template<class T>
class Selector
{
public:
    Selector();
    Selector(const QString& nameDb);
    DBTypes::DBResult selectAll(const std::string& tableName,
                                std::vector<QVariantList>& returnData);
private:
    std::unique_ptr<Executor<T>> m_executor;
    std::string generateQuery(const std::string& tableName) const;
};

}
