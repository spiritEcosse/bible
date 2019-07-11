#ifndef MOCKMODULESMODEL_H
#define MOCKMODULESMODEL_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/ModulesModel.h"
#include "mock_iqsqldatabase.h"
#include "mock_iqsqlquery.h"

template <class MockIQSqlDatabase, class MockIQSqlQuery>
class MockModulesModel : public ModulesModel<MockIQSqlDatabase, MockIQSqlQuery>
{
public:
    MockModulesModel(MockIQSqlDatabase &db, QObject *parent)
        : ModulesModel<MockIQSqlDatabase, MockIQSqlQuery>(db, parent)
    {
    }
    MockModulesModel() {
    }
    MOCK_METHOD2_T(createTable, bool(const QString &tableName, const QString &relatedTable));
    MOCK_METHOD1_T(setTable, void(const QString &tableName));
    MOCK_METHOD0_T(select, bool());
    MOCK_CONST_METHOD0_T(query, MockIQSqlQuery&());
    bool ParentCreateTable(const QString &tableName, const QString &relatedTable) { return ModulesModel<MockIQSqlDatabase, MockIQSqlQuery>::createTable(tableName, relatedTable); }
};

#endif // MOCKMODULESMODEL_H
