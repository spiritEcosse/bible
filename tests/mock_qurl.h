#ifndef MOCK_QURL_H
#define MOCK_QURL_H

#include <gmock/gmock.h>

#include <QUrl>

class MockQUrl : public QUrl
{
public:
    MOCK_METHOD(QUrl, resolved, (const QUrl &relative), (const));
    MOCK_CONST_METHOD1(toDisplayString, QString(FormattingOptions options));
    MOCK_METHOD(bool, isRelative, (), (const));
    MOCK_METHOD(QUrl, fromEncoded, (const QByteArray&, ParsingMode), (override));
    MOCK_CONST_METHOD1(path, QString(ComponentFormattingOption)); // WARNING: replace on ComponentFormattingOptions
};


#endif // MOCK_QURL_H
