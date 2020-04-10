#ifndef MOCK_QFILE_H
#define MOCK_QFILE_H

#include <gmock/gmock.h>

#include <QFile>


class MockQFile : public QFile
{
public:
    MOCK_METHOD1(setFileName, void(const QString&));
    MOCK_METHOD(bool, open, (OpenMode flags), (override));
    MOCK_METHOD0(close, void());
    MOCK_METHOD0(remove, bool());
    MOCK_CONST_METHOD0(fileName, QString());
    MOCK_METHOD1(write, qint64(const QByteArray &));
    MOCK_METHOD(bool, exists, (const QString &fileName), (override));
    MOCK_METHOD(QByteArray, readAll, ());
};

#endif // MOCK_QFILE_H
