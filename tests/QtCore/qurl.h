#ifndef QURL_H
#define QURL_H

#include <QByteArray>
#include <QString>

enum ComponentFormattingOption {
    FullyDecoded = 2
};

Q_DECLARE_FLAGS(ComponentFormattingOptions, ComponentFormattingOption);

class QUrl
{
public:
    enum ParsingMode {
        TolerantMode,
        StrictMode,
        DecodedMode
    };

    virtual ~QUrl() {}
    virtual QUrl fromEncoded(const QByteArray &url, ParsingMode mode = TolerantMode) {
        Q_UNUSED(url)
        Q_UNUSED(mode)
        return QUrl();
    }
    virtual QString path(ComponentFormattingOptions options = FullyDecoded) const {
        Q_UNUSED(options)
        return QString();
    }
};

#endif // QURL_H
