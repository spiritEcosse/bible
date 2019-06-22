#ifndef MOCK_IQSQLDATABASE_H
#define MOCK_IQSQLDATABASE_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QStringList>

#include "iqsqldatabase.h"

class MockIQSqlDatabase : public IQSqlDatabase
{
public:
    MOCK_CONST_METHOD0(tables, QStringList());
};

#endif // MOCK_IQSQLDATABASE_H
