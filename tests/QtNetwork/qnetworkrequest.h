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
    enum RedirectPolicy {
        ManualRedirectPolicy,
        NoLessSafeRedirectPolicy,
        SameOriginRedirectPolicy,
        UserVerifiedRedirectPolicy
    };
    enum KnownHeaders {
        ContentTypeHeader,
        ContentLengthHeader,
        LocationHeader,
        LastModifiedHeader,
        CookieHeader,
        SetCookieHeader,
        ContentDispositionHeader,  // added for QMultipartMessage
        UserAgentHeader,
        ServerHeader,
        IfModifiedSinceHeader,
        ETagHeader,
        IfMatchHeader,
        IfNoneMatchHeader
    };

    QNetworkRequest(const QUrl &url = QUrl()) { Q_UNUSED(url); }
    virtual QUrl url() const {
        return QUrl();
    }
};

#endif // QNETWORKREQUEST_H
