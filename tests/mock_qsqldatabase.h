#ifndef MOCK_QSQLDATABASE_H
#define MOCK_QSQLDATABASE_H

#include <gmock/gmock.h>
#include <QtCore/qstringlist.h>
#include <QtSql/qsqldatabase.h>

class MockQSqlDatabase : public QSqlDatabase
{
public:
    MockQSqlDatabase() {}
    MOCK_CONST_METHOD0(isValid, bool());
    MOCK_CONST_METHOD0(lastError, QSqlError());
    MOCK_METHOD0(open, bool());
    MOCK_CONST_METHOD1(tables, QStringList(QSql::TableType));
    MOCK_METHOD1(setDatabaseName, void(const QString& name));
};

#endif // MOCK_QSQLDATABASE_H
