#ifndef QSQLERROR_H
#define QSQLERROR_H

#include <QtSql/qtsqlglobal.h>
#include <QtCore/qstring.h>

class QSqlError
{
public:
//    enum ErrorType {
//        NoError,
//        ConnectionError,
//        StatementError,
//        TransactionError,
//        UnknownError
//    };
//    QSqlError(const QString &driverText = QString(),
//              const QString &databaseText = QString(),
//              ErrorType type = NoError,
//              const QString &errorCode = QString());
//    QSqlError(const QSqlError& other);
//    QSqlError(QSqlError &&other) Q_DECL_NOTHROW : d(other.d) { other.d = nullptr; }
//    QSqlError& operator=(const QSqlError& other);
//    QSqlError &operator=(QSqlError &&other) Q_DECL_NOTHROW { swap(other); return *this; }

//    bool operator==(const QSqlError& other) const;
//    bool operator!=(const QSqlError& other) const;
//    ~QSqlError();

//    void swap(QSqlError &other) Q_DECL_NOTHROW { qSwap(d, other.d); }

//    QString driverText() const;
//    QString databaseText() const;
//    ErrorType type() const;
//#if QT_DEPRECATED_SINCE(5, 3)
//    QT_DEPRECATED int number() const;
//#endif
//    QString nativeErrorCode() const;
//    QString text() const;
//    bool isValid() const;

//#if QT_DEPRECATED_SINCE(5, 1)
//    QT_DEPRECATED void setDriverText(const QString &driverText);
//    QT_DEPRECATED void setDatabaseText(const QString &databaseText);
//    QT_DEPRECATED void setType(ErrorType type);
//    QT_DEPRECATED void setNumber(int number);
//#endif

//private:
//    // ### Qt6: Keep the pointer and remove the rest.
//    QString unused1;
//    QString unused2;
//    struct Unused {
//        ErrorType unused3;
//        int unused4;
//    };
//    union {
//        QSqlErrorPrivate *d;
//        Unused unused5;
//    };
};

#endif // QSQLERROR_H
