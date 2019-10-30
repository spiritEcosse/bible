#ifndef MOCK_QSTRING_H
#define MOCK_QSTRING_H

#include <gmock/gmock.h>

#include <QString>


class MockQString : public QString
{
public:
    MOCK_CONST_METHOD0(toLocal8Bit, QByteArray());
};

#endif // MOCK_QSTRING_H
