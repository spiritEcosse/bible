#ifndef MOCKMODULESMODEL_H
#define MOCKMODULESMODEL_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/ModulesModel.h"
#include "mock_iqsqldatabase.h"
#include "iqsqlquery.h"

using ::testing::_;
using ::testing::Invoke;

template <class MockIQSqlDatabase>
class MockModulesModel : public ModulesModel<MockIQSqlDatabase>
{
public:
    MockModulesModel(MockIQSqlDatabase &db, QObject *parent)
        : ModulesModel<MockIQSqlDatabase>(db, parent)
    {
    }
    MockModulesModel() {
    }
    MOCK_METHOD2_T(createTable, bool(const QString &tableName, const QString &relatedTable));
    MOCK_METHOD1_T(setTable, void(const QString &tableName));
    MOCK_METHOD0_T(select, bool());
    MOCK_CONST_METHOD0_T(query, QSqlQuery());
//    QSqlQuery query() { return ModulesModel<MockIQSqlDatabase::query(); }
    bool ParentCreateTable(const QString &tableName, const QString &relatedTable) { return ModulesModel<MockIQSqlDatabase>::createTable(tableName, relatedTable); }
};

#endif // MOCKMODULESMODEL_H
