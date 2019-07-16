#ifndef MOCKMODULESGROUPMODEL_H
#define MOCKMODULESGROUPMODEL_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/ModulesGroupModel.h"
#include "mock_iqsqldatabase.h"
#include "mock_iqsqlquery.h"

template <class MockIQSqlDatabase, class MockIQSqlQuery>
class MockModulesGroupModel : public ModulesGroupModel<MockIQSqlDatabase, MockIQSqlQuery>
{
public:
    MockModulesGroupModel(MockIQSqlDatabase &db, QObject *parent = nullptr)
        : ModulesGroupModel<MockIQSqlDatabase, MockIQSqlQuery>(db, parent) {}
    MockModulesGroupModel() {}
    MOCK_METHOD0_T(init, void());
    MOCK_METHOD1_T(createTable, bool(const QString &tableName));
    MOCK_CONST_METHOD0_T(query, MockIQSqlQuery&());
    MOCK_METHOD1_T(execLastError, bool(const QString& query));
    MOCK_METHOD0_T(checkAvailabilityNewModules, void());
    MOCK_CONST_METHOD3_T(correctTitle, QString(QString, QString, QString));
    MOCK_METHOD0_T(setCountOldRows, void());
    MOCK_METHOD1_T(newRows, void(QJsonArray &downloads));
    // The following line won't really compile, as the return
    // type has multiple template arguments.  To fix it, use a
    // typedef for the return type.
    MOCK_CONST_METHOD3_T(makeGroup, QMap<QString, QString>(QString, QString, QString));
    MOCK_CONST_METHOD2_T(data, QVariant(const QModelIndex &index, int role));
    // The following line won't really compile, as the return
    // type has multiple template arguments.  To fix it, use a
    // typedef for the return type.
    MOCK_CONST_METHOD0_T(roleNames, QHash<int, QByteArray>());
    MOCK_METHOD1_T(setTable, void(const QString &tableName));
    MOCK_METHOD0_T(select, bool());
    MOCK_METHOD0_T(updateModules, void());

    bool ParentCreateTable(const QString &tableName) {
        return ModulesGroupModel<MockIQSqlDatabase, MockIQSqlQuery>::createTable(tableName);
    }
    void parentInit() {
        return ModulesGroupModel<MockIQSqlDatabase, MockIQSqlQuery>::init();
    }
    bool parentExecLastError(const QString& query) {
        return ModulesGroupModel<MockIQSqlDatabase, MockIQSqlQuery>::execLastError(query);
    }
};

#endif // MOCKMODULESGROUPMODEL_H
