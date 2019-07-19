#ifndef QNETWORKREQUEST_H
#define QNETWORKREQUEST_H

#include <QtCore/QUrl>

class QNetworkRequest
{
public:
    enum Attribute {
        HttpStatusCodeAttribute,
        RedirectionTargetAttribute
    };
    QNetworkRequest(const QUrl &url = QUrl()) {
        Q_UNUSED(url);
    }
    virtual QUrl url() const {}
};

#endif // QNETWORKREQUEST_H
