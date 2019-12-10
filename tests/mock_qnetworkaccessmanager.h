#ifndef MOCK_QNETWORKACCESSMANAGER_H
#define MOCK_QNETWORKACCESSMANAGER_H

#include <gmock/gmock.h>

#include <QNetworkAccessManager>


class MockQNetworkAccessManager : public QNetworkAccessManager
{
public:
    MOCK_METHOD1(get, QNetworkReply*(const QNetworkRequest &request));
};

#endif // MOCK_QNETWORKACCESSMANAGER_H
