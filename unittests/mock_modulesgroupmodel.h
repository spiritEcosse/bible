#ifndef MOCKMODULESGROUPMODEL_H
#define MOCKMODULESGROUPMODEL_H

#include <gmock/gmock.h>

#include "../../src/ModulesGroupModel.h"


class MockModulesGroupModel : public ModulesGroupModel
{
public:
    MOCK_METHOD0(init, void());
    MOCK_METHOD0(createTable, bool());
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
    MOCK_METHOD0(extractRegistry, void());
    MOCK_METHOD0(updateModules, void());
    MOCK_METHOD1(setEditStrategy, void(EditStrategy strategy));
    MOCK_CONST_METHOD0(record, QSqlRecord());
    MOCK_CONST_METHOD1(record, QSqlRecord(int row));
    MOCK_METHOD2(insertRecord, bool(int row, const QSqlRecord &record));
    MOCK_METHOD0(submitAll, bool());
    MOCK_METHOD0(database, QSqlDatabase&());
    MOCK_CONST_METHOD0(lastError, QSqlError&());
    MOCK_METHOD(void, updateTable, ());
    MOCK_METHOD(void, compareVersions, ());

    bool ParentCreateTable() {
        return ModulesGroupModel::createTable();
    }
    void parentInit() {
        return ModulesGroupModel::init();
    }
    bool parentExecLastError(const QString& query) {
        return ModulesGroupModel::execLastError(query);
    }
    void parentNewRows(QJsonArray &downloads) {
        return ModulesGroupModel::newRows(downloads);
    }
    void parentUpdateTable() {
        return ModulesGroupModel::updateTable();
    }
    void parentCompareVerions() {
        return ModulesGroupModel::compareVersions();
    }
    void parentUpdateModules() {
        return ModulesGroupModel::updateModules();
    }
    void parentSetCountOldRows() {
        return ModulesGroupModel::setCountOldRows();
    }
    void parentCheckAvailabilityNewModules() {
        return ModulesGroupModel::checkAvailabilityNewModules();
    }
};

#endif // MOCKMODULESGROUPMODEL_H
