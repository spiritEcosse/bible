#ifndef IQSQLDATABASE_H
#define IQSQLDATABASE_H

class IQSqlDatabase
{
public:
    virtual ~IQSqlDatabase() {}
    virtual bool open() = 0;
    virtual QStringList tables(QSql::TableType type = QSql::Tables) const;
};

#endif // IQSQLDATABASE_H
