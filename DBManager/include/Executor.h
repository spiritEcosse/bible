#pragma once
#include "ConnectionManager.h"
#include "dbtypes.h"
#include <QSqlQuery>
#include <QVariantList>


namespace db
{
class Executor
{
public:
    Executor();

    std::pair<DBResult, QSqlQuery> execute(const QString& queryText, const QVariantList& args = {});

private:
    std::unique_ptr<ConnectionManager> m_connectionManager;
};

}
