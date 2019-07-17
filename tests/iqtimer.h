#ifndef IQTIMER_H
#define IQTIMER_H

class QTimer
{
public:
    virtual void singleShot(int msec, const QObject *receiver, const char *member) = 0;
};

#endif // IQTIMER_H
