#ifndef MOCK_QSTRING_H
#define MOCK_QSTRING_H

#include <gmock/gmock.h>

#include <QString>
#include <QByteArray>


class MockQstring : public QString
{
public:
    MOCK_METHOD0(toLocal8Bit, QByteArray());
};

#endif // MOCK_QSTRING_H
