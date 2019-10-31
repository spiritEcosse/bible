#ifndef MOCK_QURL_H
#define MOCK_QURL_H

#include <gmock/gmock.h>

#include <QUrl>

class MockQUrl : public QUrl
{
public:
    MOCK_CONST_METHOD1(toDisplayString, QString(FormattingOptions options));
    MOCK_CONST_METHOD0(isRelative, bool());
    MOCK_METHOD2(fromEncodedImpl, QUrl(QByteArray, ParsingMode));
    MOCK_CONST_METHOD1(path, QString(ComponentFormattingOption)); // WARNING: replace on ComponentFormattingOptions
    virtual QUrl fromEncoded(
            const QByteArray &url, ParsingMode mode = TolerantMode)
    {
        return fromEncodedImpl(url, mode);
    }
};


#endif // MOCK_QURL_H
