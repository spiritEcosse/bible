#ifndef QSQLQUERY_H
#define QSQLQUERY_H

#include <QSqlError>
#include <QVariant>


class QSqlQuery
{
public:
    virtual ~QSqlQuery() {}

    QVariant* qVariant;
    QSqlError* error;
    inline virtual QSqlError& lastError() const {
        return *error;
    }
    inline virtual bool exec(const QString& query) {
        Q_UNUSED(query)
        return true;
    }
    inline virtual bool exec() {
        return true;
    }
    inline virtual bool first() {
        return true;
    }
    inline virtual QVariant& value(const QString&) const {
        return *qVariant;
    }
};

#endif // QSQLQUERY_H
