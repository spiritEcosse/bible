#ifndef QSQLDATABASE_H
#define QSQLDATABASE_H

#include <QtSql/qtsqlglobal.h>
#include <QtCore/qstring.h>

class QSqlError;

class QSqlDatabase
{
public:
    QSqlDatabase();
    QSqlDatabase(const QSqlDatabase &other);
    virtual ~QSqlDatabase();

    static const char *defaultConnection;
    virtual bool isValid() const {}
    static QSqlDatabase database(const QString& connectionName = QLatin1String(defaultConnection),
                                 bool open = true) {}
    static QSqlDatabase addDatabase(const QString& type,
                                     const QString& connectionName = QLatin1String(defaultConnection));
    virtual QSqlError lastError() const;

//    QSqlDatabase &operator=(const QSqlDatabase &other);

    virtual bool open() {}
//    bool open(const QString& user, const QString& password);
//    void close();
//    bool isOpen() const;
//    bool isOpenError() const;
    virtual QStringList tables(QSql::TableType type = QSql::Tables) const;
//    QSqlIndex primaryIndex(const QString& tablename) const;
//    QSqlRecord record(const QString& tablename) const;
//    QSqlQuery exec(const QString& query = QString()) const;
//    QSqlError lastError() const;



//    bool transaction();
//    bool commit();
//    bool rollback();

    virtual void setDatabaseName(const QString& name);
//    void setUserName(const QString& name);
//    void setPassword(const QString& password);
//    void setHostName(const QString& host);
//    void setPort(int p);
//    void setConnectOptions(const QString& options = QString());
//    QString databaseName() const;
//    QString userName() const;
//    QString password() const;
//    QString hostName() const;
//    QString driverName() const;
//    int port() const;
//    QString connectOptions() const;
//    QString connectionName() const;
//    void setNumericalPrecisionPolicy(QSql::NumericalPrecisionPolicy precisionPolicy);
//    QSql::NumericalPrecisionPolicy numericalPrecisionPolicy() const;

//    QSqlDriver* driver() const;

//    static QSqlDatabase addDatabase(const QString& type,
//                                 const QString& connectionName = QLatin1String(defaultConnection));
//    static QSqlDatabase addDatabase(QSqlDriver* driver,
//                                 const QString& connectionName = QLatin1String(defaultConnection));
//    static QSqlDatabase cloneDatabase(const QSqlDatabase &other, const QString& connectionName);
//    static QSqlDatabase cloneDatabase(const QString &other, const QString& connectionName);

    //    static void removeDatabase(const QString& connectionName);
    static bool contains(const QString& connectionName = QLatin1String(defaultConnection));
//    static QStringList drivers();
//    static QStringList connectionNames();
//    static void registerSqlDriver(const QString &name, QSqlDriverCreatorBase *creator);
//    static bool isDriverAvailable(const QString &name);

//protected:
//    explicit QSqlDatabase(const QString& type);
//    explicit QSqlDatabase(QSqlDriver* driver);

//private:
//    friend class QSqlDatabasePrivate;
//    QSqlDatabasePrivate *d;
};

#endif // QSQLDATABASE_H
