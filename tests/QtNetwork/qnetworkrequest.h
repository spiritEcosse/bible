#ifndef QNETWORKREQUEST_H
#define QNETWORKREQUEST_H

#include <QUrl>

class QNetworkRequest
{
public:
    enum Attribute {
        HttpStatusCodeAttribute,
        RedirectionTargetAttribute
    };

    QNetworkRequest(const QUrl &url = QUrl()) { Q_UNUSED(url); }
    virtual QUrl url() const {
        return QUrl();
    }
};

#endif // QNETWORKREQUEST_H
