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
    void executeBatch(const std::string& queryText, std::vector<T>& container);

private:
    std::unique_ptr<ConnectionManager> m_connectionManager;
};

}
