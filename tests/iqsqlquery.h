#ifndef IQSQLQUERY_H
#define IQSQLQUERY_H

#include <QSqlQuery>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class IQSqlQuery : public QSqlQuery
{
public:
    virtual ~IQSqlQuery() {}
    virtual bool exec(const QString& query) = 0;
};

#endif // IQSQLQUERY_H
