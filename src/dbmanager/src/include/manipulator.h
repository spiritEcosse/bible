#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "Executor.h"
#include "dbtypes.h"

using namespace DBTypes;

namespace db
{
template<class T>
class Manipulator
{
public:
    Manipulator();
    std::pair<DBResult, DBIndex> insertRow(const std::string& tableName,
                                           const QVariantList& recordData);
    void insertBulk(const T& container);
private:
    std::unique_ptr<Executor> m_executor;
    std::string generateBindString(size_t recordSize) const;
    std::string generateInsertQuery(const std::string& tableName, size_t recordSize) const;
};
}

#endif // MANIPULATOR_H
