#ifndef MOCK_QJSONDOCUMENT_H
#define MOCK_QJSONDOCUMENT_H

#include <gmock/gmock.h>

#include <QJsonDocument>

class MockQJsonParseError : public QJsonParseError
{
public:

};

class MockQJsonDocument : public QJsonDocument
{
public:
    MOCK_METHOD(QJsonDocument&, fromJson, (const QByteArray &json, QJsonParseError *error));
    MOCK_METHOD(QJsonObject&, object, (), (const));
};

#endif // MOCK_QJSONDOCUMENT_H
