#pragma once
#include "Executor.h"
#include "dbtypes.h"

using namespace DBTypes;

namespace db
{
class Manipulator
{
public:
    Manipulator();
    std::pair<DBResult, DBIndex> insertRow(const std::string& tableName,
                                           const QVariantList& recordData);
private:
    std::unique_ptr<Executor> m_executor;
    std::string generateBindString(size_t recordSize) const;
    std::string generateInsertQuery(const std::string& tableName, size_t recordSize) const;
};
}
