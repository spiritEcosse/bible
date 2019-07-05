#ifndef MOCKMODULESMODEL_H
#define MOCKMODULESMODEL_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/ModulesModel.h"
#include "mock_iqsqldatabase.h"

template <class MockIQSqlDatabase>
class MockModulesModel : public ModulesModel<MockIQSqlDatabase>
{
public:
    MockModulesModel(MockIQSqlDatabase &db, QObject *parent)
        : ModulesModel<MockIQSqlDatabase>(db, parent), db_(&db) {}
    MockModulesModel() {}
    MOCK_METHOD2_T(createTable, bool(const QString &tableName, const QString &relatedTable));
    MOCK_METHOD1_T(setTable, void(const QString &tableName));
    MOCK_METHOD0_T(select, bool());
    MockIQSqlDatabase *db_;
    MOCK_METHOD0_T(query, QSqlQuery());
};

#endif // MOCKMODULESMODEL_H
