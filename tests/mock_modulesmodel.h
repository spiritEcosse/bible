//#ifndef MOCKMODULESMODEL_H
//#define MOCKMODULESMODEL_H

//#include <gmock/gmock.h>
//#include <gtest/gtest.h>

//#include "../src/ModulesModel.h"
//#include "mock_iqsqldatabase.h"
//#include "mock_iqsqlquery.h"

//template <class MockIQSqlDatabase, class MockIQSqlQuery>
//class MockModulesModel : public ModulesModel<MockIQSqlDatabase, MockIQSqlQuery>
//{
//public:
//    MockModulesModel(MockIQSqlDatabase &db, QObject *parent = nullptr)
//        : ModulesModel<MockIQSqlDatabase, MockIQSqlQuery>(db, parent) {}
//    MockModulesModel() {}
//    MOCK_CONST_METHOD2_T(data, QVariant(const QModelIndex &index, int role));
//    MOCK_METHOD0_T(init, void());
//    MOCK_METHOD2_T(createTable, bool(const QString &tableName, const QString &relatedTable));
//    MOCK_CONST_METHOD0_T(query, MockIQSqlQuery&());
//    MOCK_METHOD1_T(execLastError, bool(const QString& query));
//    MOCK_METHOD1_T(setTable, void(const QString &tableName));
//    MOCK_METHOD0_T(select, bool());
//    MOCK_CONST_METHOD0_T(record, QSqlRecord());
//    MOCK_CONST_METHOD1_T(record, QSqlRecord(int row));
//    MOCK_METHOD2_T(insertRecord, bool(int row, const QSqlRecord &record));
//    MOCK_METHOD0_T(submitAll, bool());

//    bool ParentCreateTable(const QString &tableName, const QString &relatedTable) {
//        return ModulesModel<MockIQSqlDatabase, MockIQSqlQuery>::createTable(tableName, relatedTable);
//    }
//    void parentInit() {
//        return ModulesModel<MockIQSqlDatabase, MockIQSqlQuery>::init();
//    }
//    bool parentExecLastError(const QString& query) {
//        return ModulesModel<MockIQSqlDatabase, MockIQSqlQuery>::execLastError(query);
//    }
//};

//#endif // MOCKMODULESMODEL_H
