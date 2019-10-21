#ifndef MOCK_QSTRINGLIST_H
#define MOCK_QSTRINGLIST_H

#include <gmock/gmock.h>

#include <QStringList>

class MockQStringList : public QStringList
{
public:
    MOCK_METHOD1(append, void(const QString&));
};

#endif // MOCK_QSTRINGLIST_H
