#ifndef MOCK_IQSQLQUERY_H
#define MOCK_IQSQLQUERY_H

#include <gmock/gmock.h>

#include <QString>
#include <QSqlError>
#include <QSqlQuery>

class MockQSqlQuery : public QSqlQuery
{
public:
    MOCK_CONST_METHOD0(lastError, QSqlError&());
    MOCK_METHOD1(exec, bool(const QString& query));
    MOCK_METHOD0(exec, bool());
    MOCK_METHOD(bool, first, ());
    MOCK_METHOD(QVariant&, value, (const QString&), (const));
};

#endif // MOCK_IQSQLQUERY_H
