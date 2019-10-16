#ifndef QQUEUE_H
#define QQUEUE_H

template <class T>
class QQueue
{
public:
    virtual ~QQueue() {}
    virtual bool isEmpty() const = 0;
    virtual void enqueue(const T &) = 0;
};

#endif // QQUEUE_H
