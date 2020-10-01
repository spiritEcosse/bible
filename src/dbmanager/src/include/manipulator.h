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
    void insertBulk(const std::vector<T>& container);
private:
    std::unique_ptr<Executor<T>> m_executor;
    std::string generateBindString(size_t recordSize) const;
    QString generateInsertQuery(size_t recordSize) const;
};
}

#endif // MANIPULATOR_H
