#ifndef MOCK_QTIME_H
#define MOCK_QTIME_H

#include <gmock/gmock.h>

#include <QTime>


class MockQTime : public QTime
{
public:
    MOCK_METHOD(void, start, ());
};

#endif // MOCK_QTIME_H
