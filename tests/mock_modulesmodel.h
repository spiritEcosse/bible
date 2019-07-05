#ifndef MOCKMODULESMODEL_H
#define MOCKMODULESMODEL_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/ModulesModel.h"
#include "mock_iqsqldatabase.h"

using ::testing::_;
using ::testing::Invoke;

template <class MockIQSqlDatabase>
class MockModulesModel : public ModulesModel<MockIQSqlDatabase>
{
public:
    MockModulesModel(MockIQSqlDatabase &db, QObject *parent)
        : ModulesModel<MockIQSqlDatabase>(db, parent)
    {
//        ON_CALL(*this, createTable(_, _))
//                .WillByDefault(Invoke(&real_, &ModulesModel<MockIQSqlDatabase>::createTable));
    }
    MockModulesModel() {}
//    MOCK_METHOD2_T(createTable, bool(const QString &tableName, const QString &relatedTable));
    MOCK_METHOD1_T(setTable, void(const QString &tableName));
    MOCK_METHOD0_T(select, bool());
    MOCK_METHOD0_T(query, QSqlQuery());
private:
    ModulesModel<MockIQSqlDatabase> real_;
};

#endif // MOCKMODULESMODEL_H
