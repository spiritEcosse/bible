#ifndef QSQLERROR_H
#define QSQLERROR_H

#include <QtSql/qtsqlglobal.h>
#include <QtCore/qstring.h>

class QSqlError
{
public:
    QSqlError() {}
    virtual ~QSqlError() {}
    virtual QString text() const {}
};

#endif // QSQLERROR_H
