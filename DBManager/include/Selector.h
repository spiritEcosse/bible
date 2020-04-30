#pragma once
#include "dbtypes.h"
#include <QString>
#include <vector>
#include <QVariantList>
#include "Executor.h"

namespace db
{
class Selector
{
public:
    std::pair<DBResult, std::vector<DBEntry>> selectAll(const QString& tableName);

private:
    Executor m_executor;
    QString generateQuery(const QString& tableName) const;
};

}
