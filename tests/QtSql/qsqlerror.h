#ifndef QSQLERROR_H
#define QSQLERROR_H

#include <QtCore/qstring.h>

class QSqlError
{
public:
    virtual ~QSqlError() {}
    virtual QString text() const {
        return QString("");
    }
};

#endif // QSQLERROR_H
