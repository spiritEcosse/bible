#ifndef QTIMER_H
#define QTIMER_H

#include <QtCore/qobject.h>

class QTimer
{
public:
    virtual ~QTimer() {}
    virtual void singleShot(int msec, const QObject *receiver, const char *member) = 0;
};

#endif // QTIMER_H
