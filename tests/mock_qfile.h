#ifndef MOCK_QFILE_H
#define MOCK_QFILE_H

#include <gmock/gmock.h>

#include <QFile>


class MockQFile : public QFile
{
public:
    MOCK_METHOD1(setFileName, void(const QString&));
};

#endif // MOCK_QFILE_H
