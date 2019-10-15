#ifndef QSQLQUERY_H
#define QSQLQUERY_H

#include <QSqlError>

class QSqlQuery
{
public:
    virtual ~QSqlQuery() {}

    virtual QSqlError& lastError() const = 0;
    virtual bool exec(const QString& query) = 0;
    virtual bool exec() = 0;
};

#endif // QSQLQUERY_H
