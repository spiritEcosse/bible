#ifndef MOCK_IQSQLQUERY_H
#define MOCK_IQSQLQUERY_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "iqsqlquery.h"

class MockIQSqlQuery : public IQSqlQuery
{
public:
    MockIQSqlQuery() {}
    MOCK_METHOD1(exec, bool(const QString& query));
};


#endif // MOCK_IQSQLQUERY_H
