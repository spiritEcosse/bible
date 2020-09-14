#pragma once
#include "dbtypes.h"
#include <vector>
#include <QVariantList>
#include <memory>

namespace db
{
class Executor;

class Selector
{
public:
    Selector();
    Selector(const QString& nameDb);
    DBTypes::DBResult selectAll(const std::string& tableName,
                                std::vector<QVariantList>& returnData);
private:
    std::unique_ptr<Executor> m_executor;
    std::string generateQuery(const std::string& tableName) const;
};

}