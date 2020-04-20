#ifndef MOCK_QLIST_H
#define MOCK_QLIST_H

#include <gmock/gmock.h>

#include <QList>

template <typename T>
class MockQList : public QList<T>
{
public:
//    MOCK_METHOD(T&, last, (), (const));
};

#endif // MOCK_QLIST_H
