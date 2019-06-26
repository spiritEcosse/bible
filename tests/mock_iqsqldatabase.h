#ifndef MOCK_IQSQLDATABASE_H
#define MOCK_IQSQLDATABASE_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QStringList>

#include "iqsqldatabase.h"
#include <QtSql/qtsqlglobal.h>

class MockIQSqlDatabase
{
public:
    MockIQSqlDatabase() {}
    MOCK_CONST_METHOD1(tables, QStringList(QSql::TableType type));
};

#endif // MOCK_IQSQLDATABASE_H
