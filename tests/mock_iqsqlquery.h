#ifndef MOCK_IQSQLQUERY_H
#define MOCK_IQSQLQUERY_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "iqsqlquery.h"

class MockIQSqlQuery : public IQSqlQuery
{
public:
    MOCK_METHOD1(exec, bool(const QString& query));
    MOCK_CONST_METHOD0(lastError, QSqlError());
};

#endif // MOCK_IQSQLQUERY_H
