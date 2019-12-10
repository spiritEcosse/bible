#ifndef MOCK_QNETWORKREQUEST_H
#define MOCK_QNETWORKREQUEST_H

#include <gmock/gmock.h>

#include <QNetworkRequest>

class MockQNetworkRequest : public QNetworkRequest
{
public:
    MOCK_METHOD0(url, QUrl&());
    MOCK_METHOD(void, setUrl, (const QUrl &url), (override));
};

#endif // MOCK_QNETWORKREQUEST_H
