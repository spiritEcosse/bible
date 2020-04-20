#ifndef MOCK_QTIMER_H
#define MOCK_QTIMER_H

#include <gmock/gmock.h>

#include <QTimer>

class MockQTimer : public QTimer
{
public:
    MOCK_METHOD3(singleShot,
                 void(int msec, const QObject *receiver, const char *member));
};

#endif // MOCK_QTIMER_H
