#ifndef MOCK_QSTRINGLIST_H
#define MOCK_QSTRINGLIST_H

#include <gmock/gmock.h>

#include <QStringList>

class MockQStringList : public QStringList
{
public:
    MOCK_METHOD1(append, void(const QString&));
    MOCK_CONST_METHOD2(containsImpl, bool(QString, Qt::CaseSensitivity));
};

#endif // MOCK_QSTRINGLIST_H
