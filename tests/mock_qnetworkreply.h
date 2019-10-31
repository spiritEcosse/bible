#ifndef MOCK_QNETWORKREPLY_H
#define MOCK_QNETWORKREPLY_H

#include <gmock/gmock.h>

#include <QNetworkReply>


class MockQNetworkReply : public QNetworkReply
{
public:
    MOCK_METHOD0(close, void());
    MOCK_METHOD0(readAll, QByteArray());
    MOCK_CONST_METHOD0(isSequential, bool());
    MOCK_METHOD1(setReadBufferSize, void(qint64));
    MOCK_METHOD0(abort, void());
    MOCK_METHOD0(ignoreSslErrors, void());
    MOCK_METHOD2(readData, qint64(char *data, qint64 maxlen));
    MOCK_METHOD2(readLineData, qint64(char *data, qint64 maxlen));
    MOCK_CONST_METHOD1(attribute, QVariant&(QNetworkRequest::Attribute code));
    MOCK_METHOD0(request, QNetworkRequest&());
};

#endif // MOCK_QNETWORKREPLY_H
