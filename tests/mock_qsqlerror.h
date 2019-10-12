#ifndef MOCK_QSQLERROR_H
#define MOCK_QSQLERROR_H

#include <gmock/gmock.h>
#include <QtSql/qsqlerror.h>

class MockQSqlError : public QSqlError
{
public:
    MockQSqlError() {}
    MOCK_CONST_METHOD0(text, QString());
};

#endif // MOCK_QSQLERROR_H
