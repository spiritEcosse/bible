#pragma once
#include "Executor.h"
#include "dbtypes.h"

namespace db
{
class Manipulator
{
public:
    Manipulator();
    Manipulator(const QString& nameDb);
    std::pair<DBResult, DBIndex> insertRow(const QString& tableName,
                                           const QVariantList& recordData);
private:
    std::unique_ptr<Executor> m_executor;
    QString generateBindString(size_t recordSize) const;
    QString generateInsertQuery(const QString& tableName, size_t recordSize) const;
};
}
