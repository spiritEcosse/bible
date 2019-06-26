#ifndef IQSQLDATABASE_H
#define IQSQLDATABASE_H

#include <qsqldatabase.h>

class IQSqlDatabase : public QSqlDatabase
{
public:
    virtual ~IQSqlDatabase() {}
//    virtual bool open() = 0;
    virtual int tables(QSql::TableType type) const = 0;
//    virtual int Pop() = 0;
};

#endif // IQSQLDATABASE_H
