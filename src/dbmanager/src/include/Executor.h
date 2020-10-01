#pragma once
#include "dbtypes.h"
#include <QSqlQuery>
#include <QVariantList>
#include "ConnectionManager.h"

using namespace DBTypes;

namespace db
{
template<class T>
class Executor
{
public:
    Executor();
    Executor(const QString& nameDb);

    std::pair<DBResult, QSqlQuery> execute(const std::string& queryText, const QVariantList& args = {});
    std::pair<DBResult, QSqlQuery> executeB();

private:
    std::unique_ptr<ConnectionManager> m_connectionManager;
};

}
