#ifndef MOCK_QBYTEARRAY_H
#define MOCK_QBYTEARRAY_H

#include <gmock/gmock.h>

#include <QByteArray>

class MockQByteArray : public QByteArray
{
public:
    MOCK_METHOD(QByteArray, fromBase64, (const QByteArray &base64));
};

#endif // MOCK_QBYTEARRAY_H
