#ifndef MOCK_QSQLDATABASE_H
#define MOCK_QSQLDATABASE_H

#include <gmock/gmock.h>
#include <QtCore/qstringlist.h>
#include <QSqlDatabase>

class MockQSqlDatabase : public QSqlDatabase
{
public:
    MOCK_CONST_METHOD0(isValid, bool());
    MOCK_CONST_METHOD0(lastError, QSqlError());
    MOCK_METHOD0(open, bool());
    MOCK_CONST_METHOD0(tables, QStringList());
    MOCK_METHOD1(setDatabaseName, void(const QString& name));
};

#endif // MOCK_QSQLDATABASE_H
