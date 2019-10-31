#ifndef MOCK_QNETWORKREQUEST_H
#define MOCK_QNETWORKREQUEST_H

#include <gmock/gmock.h>

#include <QNetworkRequest>

class MockQNetworkRequest : public QNetworkRequest
{
public:
    MOCK_CONST_METHOD0(url, QUrl());
};

#endif // MOCK_QNETWORKREQUEST_H
