#ifndef MOCK_IQTIMER_H
#define MOCK_IQTIMER_H

#include "iqtimer.h"

class MockIQTimer : public QTimer
{
public:
    MOCK_METHOD3(singleShot,
                 void(int msec, const QObject *receiver, const char *member));
};

#endif // MOCK_IQTIMER_H
