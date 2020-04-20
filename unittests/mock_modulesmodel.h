#ifndef MOCKMODULESMODEL_H
#define MOCKMODULESMODEL_H

#include <gmock/gmock.h>

#include "../../src/ModulesModel.h"


class MockModulesModel : public ModulesModel
{
public:
    MOCK_CONST_METHOD2(data, QVariant(const QModelIndex &index, int role));
    MOCK_METHOD0(init, void());
    MOCK_METHOD1(setEditStrategy, void(EditStrategy strategy));
    MOCK_METHOD0(createTable, bool());
    MOCK_METHOD1(execLastError, bool(const QString& query));
    MOCK_METHOD1(setTable, void(const QString &tableName));
    MOCK_METHOD0(select, bool());
    MOCK_CONST_METHOD0(record, QSqlRecord());
    MOCK_CONST_METHOD1(record, QSqlRecord(int row));
    MOCK_METHOD2(insertRecord, bool(int row, const QSqlRecord &record));
    MOCK_METHOD0(submitAll, bool());
    MOCK_METHOD0(database, QSqlDatabase&());

    bool ParentCreateTable() {
        return ModulesModel::createTable();
    }
    void parentInit() {
        return ModulesModel::init();
    }
    bool parentExecLastError(const QString& query) {
        return ModulesModel::execLastError(query);
    }
};

#endif // MOCKMODULESMODEL_H
