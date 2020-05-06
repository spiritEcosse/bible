#pragma once
#include "dbtypes.h"
#include <QSqlQuery>
#include <QVariantList>
#include "ConnectionManager.h"

class ConnectionManager;

namespace db
{
class Executor
{
public:
    Executor();
    Executor(const QString& nameDb);

    std::pair<DBResult, QSqlQuery> execute(const QString& queryText, const QVariantList& args = {});

private:
    std::unique_ptr<ConnectionManager> m_connectionManager;
};

}
