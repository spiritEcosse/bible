#ifndef MOCK_QJSONVALUE_H
#define MOCK_QJSONVALUE_H

#include <gmock/gmock.h>

#include <QJsonValue>
#include <QJsonArray>

class MockQJsonValue : public QJsonValue
{
public:
    MOCK_METHOD(QJsonArray&, toArray, (), (const));
    MOCK_METHOD(int, toInt, (int defaultValue), (const));
};

#endif // MOCK_QJSONVALUE_H
