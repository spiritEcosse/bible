#ifndef MOCK_QJSONOBJECT_H
#define MOCK_QJSONOBJECT_H

#include <gmock/gmock.h>

#include <QJsonObject>

class MockQJsonObject : public QJsonObject
{
public:
    MOCK_METHOD(QJsonValue&, value, (QLatin1String key), (const));
    MOCK_METHOD(QJsonValue&, value, (const QString &key), (const));
};

#endif // MOCK_QJSONOBJECT_H
