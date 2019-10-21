#ifndef MOCK_QSTRINGLIST_H
#define MOCK_QSTRINGLIST_H

#include <QStringList>

template <typename T>
class MockQStringList : public QStringList<T>
{
public:

};

#endif // MOCK_QSTRINGLIST_H
