#ifndef QSQLERROR_H
#define QSQLERROR_H

#include <QtSql/qtsqlglobal.h>
#include <QtCore/qstring.h>

class QSqlError
{
public:
    virtual ~QSqlError();
    virtual QString text() const;
};

#endif // QSQLERROR_H
