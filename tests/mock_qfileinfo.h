#ifndef MOCK_QFILEINFO_H
#define MOCK_QFILEINFO_H

#include <gmock/gmock.h>

#include <QFileInfo>


class MockQFileInfo : public QFileInfo
{
public:
    MOCK_CONST_METHOD0(fileName, QString());
    MOCK_METHOD1(setFile, void(const QString &file));
};

#endif // MOCK_QFILEINFO_H
