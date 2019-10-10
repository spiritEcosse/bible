#ifndef MOCKMODULESGROUPMODEL_H
#define MOCKMODULESGROUPMODEL_H

#include <gmock/gmock.h>

#include "../src/ModulesGroupModel.h"
#include "mock_qsqlquery.h"
#include "mock_qsqlrecord.h"

using ::testing::Invoke;

class MockModulesGroupModel : public ModulesGroupModel
{
public:
    MOCK_METHOD0(init, void());
    MOCK_METHOD1(createTable, bool(const QString &tableName));
    MOCK_CONST_METHOD0(query, MockQSqlQuery&());
    MOCK_METHOD1(execLastError, bool(const QString& query));
    MOCK_METHOD0(checkAvailabilityNewModules, void());
    MOCK_CONST_METHOD3(correctTitle, QString(QString, QString, QString));
    MOCK_METHOD0(setCountOldRows, void());
    MOCK_METHOD1(newRows, void(QJsonArray &downloads));
    // The following line won't really compile, as the return
    // type has multiple template arguments.  To fix it, use a
    // typedef for the return type.
    MOCK_CONST_METHOD3(makeGroup, QMap<QString, QString>(QString, QString, QString));
    MOCK_CONST_METHOD2(data, QVariant(QModelIndex, int));
    // The following line won't really compile, as the return
    // type has multiple template arguments.  To fix it, use a
    // typedef for the return type.
    MOCK_CONST_METHOD0(roleNames, QHash<int, QByteArray>());
    MOCK_METHOD1(setTable, void(const QString &tableName));
    MOCK_METHOD0(select, bool());
    MOCK_METHOD0(updateModules, void());

//    MOCK_CONST_METHOD0(record, MockQSqlRecord());
//    MOCK_CONST_METHOD1(record, MockQSqlRecord(int row));
    MOCK_METHOD2(insertRecord, bool(int row, const QSqlRecord &record));
    MOCK_METHOD0(submitAll, bool());

    bool ParentCreateTable(const QString &tableName) {
        return ModulesGroupModel::createTable(tableName);
    }
    void parentInit() {
        return ModulesGroupModel::init();
    }
    bool parentExecLastError(const QString& query) {
        return ModulesGroupModel::execLastError(query);
    }
};

#endif // MOCKMODULESGROUPMODEL_H
