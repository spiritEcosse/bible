#ifndef MOCK_QVARIANT_H
#define MOCK_QVARIANT_H

#include <gmock/gmock.h>

#include <QVariant>


class MockQVariant : public QVariant
{
public:
    MOCK_CONST_METHOD1(toInt, int(bool *ok));
    MOCK_METHOD0(toUrl, QUrl&());
    MOCK_CONST_METHOD0(isValid, bool());
};

#endif // MOCK_QVARIANT_H
