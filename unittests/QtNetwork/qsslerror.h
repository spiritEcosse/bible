#ifndef QSSLERROR_H
#define QSSLERROR_H

#include <QtNetwork/qtnetworkglobal.h>
#include <QtCore/qvariant.h>
#include <QtNetwork/qsslcertificate.h>

QT_BEGIN_NAMESPACE


#ifndef QT_NO_SSL

//class QSslErrorPrivate;
class Q_NETWORK_EXPORT QSslError
{
public:
    enum SslError {
        NoError,
        UnableToGetIssuerCertificate,
        UnableToDecryptCertificateSignature,
        UnableToDecodeIssuerPublicKey,
        CertificateSignatureFailed,
        CertificateNotYetValid,
        CertificateExpired,
        InvalidNotBeforeField,
        InvalidNotAfterField,
        SelfSignedCertificate,
        SelfSignedCertificateInChain,
        UnableToGetLocalIssuerCertificate,
        UnableToVerifyFirstCertificate,
        CertificateRevoked,
        InvalidCaCertificate,
        PathLengthExceeded,
        InvalidPurpose,
        CertificateUntrusted,
        CertificateRejected,
        SubjectIssuerMismatch, // hostname mismatch?
        AuthorityIssuerSerialNumberMismatch,
        NoPeerCertificate,
        HostNameMismatch,
        NoSslSupport,
        CertificateBlacklisted,
        CertificateStatusUnknown,
        OcspNoResponseFound,
        OcspMalformedRequest,
        OcspMalformedResponse,
        OcspInternalError,
        OcspTryLater,
        OcspSigRequred,
        OcspUnauthorized,
        OcspResponseCannotBeTrusted,
        OcspResponseCertIdUnknown,
        OcspResponseExpired,
        OcspStatusUnknown,
        UnspecifiedError = -1
    };

    // RVCT compiler in debug build does not like about default values in const-
    // So as an workaround we define all constructor overloads here explicitly
    QSslError() {}
    QSslError(SslError error);
    QSslError(SslError error, const QSslCertificate &certificate);

    QSslError(const QSslError &other) {}

    void swap(QSslError &other) Q_DECL_NOTHROW
    {}

    ~QSslError() {}
#ifdef Q_COMPILER_RVALUE_REFS
    QSslError &operator=(QSslError &&other) Q_DECL_NOTHROW { swap(other); return *this; }
#endif
    QSslError &operator=(const QSslError &other);
    bool operator==(const QSslError &other) const;
    inline bool operator!=(const QSslError &other) const
    { return !(*this == other); }

    SslError error() const;
    QString errorString() const;
    QSslCertificate certificate() const;

private:
//    QScopedPointer<QSslErrorPrivate> d;
};
Q_DECLARE_SHARED(QSslError)

Q_NETWORK_EXPORT uint qHash(const QSslError &key, uint seed = 0) Q_DECL_NOTHROW;

#ifndef QT_NO_DEBUG_STREAM
class QDebug;
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, const QSslError &error);
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, const QSslError::SslError &error);
#endif

#endif // QT_NO_SSL

QT_END_NAMESPACE

#ifndef QT_NO_SSL
Q_DECLARE_METATYPE(QList<QSslError>)
#endif

#endif
