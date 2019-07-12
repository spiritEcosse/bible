#ifndef IQSQLQUERY_H
#define IQSQLQUERY_H

#include <QSqlError>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class IQSqlQuery
{
public:
    virtual ~IQSqlQuery() {}
    virtual bool exec(const QString& query) = 0;
    virtual QSqlError lastError() const = 0;
};

#endif // IQSQLQUERY_H
