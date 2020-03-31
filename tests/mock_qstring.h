#ifndef MOCK_QSTRING_H
#define MOCK_QSTRING_H

#include <gmock/gmock.h>

#include <QString>

class MockQString : public QString
{
public:
    MOCK_METHOD(QByteArray, toLocal8Bit, (), (const));
    MOCK_METHOD2(fromLatin1, QString&(const char *str, int size));
    MOCK_CONST_METHOD0(isEmpty, bool());
    MOCK_METHOD(QString, number, (int, int base));
    MOCK_METHOD(QString&, arg, (double a, int fieldWidth, char fmt, int prec, QChar fillChar), (const));
    MOCK_METHOD(QString, arg, (const QString a, int fieldWidth, QChar fillChar), (const));
    MOCK_METHOD(QString, arg, (const QString &a1, const QString &a2), (const));
};

#endif // MOCK_QSTRING_H
