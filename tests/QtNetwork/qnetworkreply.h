#ifndef QNETWORKREPLY_H
#define QNETWORKREPLY_H

#include <QtNetwork/QNetworkRequest>

class QNetworkReply
{
public:
    enum NetworkError {};
    virtual NetworkError error() const = 0;
    virtual QString errorString() const = 0;
    virtual void deleteLater() = 0;
    virtual QByteArray readAll() = 0;
    virtual QVariant attribute(QNetworkRequest::Attribute code) const = 0;
    virtual QNetworkRequest request() const = 0;
};

#endif // QNETWORKREPLY_H
