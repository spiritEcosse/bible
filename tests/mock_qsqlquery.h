#ifndef MOCK_IQSQLQUERY_H
#define MOCK_IQSQLQUERY_H

#include <gmock/gmock.h>
#include "qsqlquery.h"
#include <QSqlError>

class MockQSqlQuery : public QSqlQuery
{
public:
    MockQSqlQuery() {}
    MOCK_CONST_METHOD0(lastError, QSqlError&());
    MOCK_METHOD1(exec, bool(const QString& query));
    MOCK_METHOD0(exec, bool());
};

#endif // MOCK_IQSQLQUERY_H
