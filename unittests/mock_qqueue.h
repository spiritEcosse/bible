#ifndef MOCK_QQUEUE_H
#define MOCK_QQUEUE_H

#include <gmock/gmock.h>

#include <QQueue>

template <typename T>
class MockQQueue : public QQueue<T> {
 public:
  MOCK_CONST_METHOD0_T(isEmpty, bool());
  MOCK_METHOD1_T(enqueue, void(const T &));
  MOCK_METHOD0_T(dequeue, T());
};

#endif // MOCK_QQUEUE_H
