#ifndef QQUEUE_H
#define QQUEUE_H

#include <QList>


template <class T>
class QQueue : public QList<T>
{
public:
    virtual ~QQueue() {}
    virtual bool isEmpty() const { return true; }
    virtual void enqueue(const T &) {}
    virtual T dequeue() { return QList<T>::takeFirst(); }
};

#endif // QQUEUE_H
