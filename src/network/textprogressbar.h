#ifndef TEXTPROGRESSBAR_H
#define TEXTPROGRESSBAR_H

//#include <gtest/gtest_prod.h>
#include <QString>

class TextProgressBar
{
public:
    virtual ~TextProgressBar() {}
    virtual void clear();
    virtual void update();
    virtual void setMessage(const QString &message);
    virtual void setStatus(qint64 value, qint64 maximum);

private:
    QString message;
    qint64 value = 0;
    qint64 maximum = -1;
    int iteration = 0;

    friend class TextProgressBarTest;
//    FRIEND_TEST(TextProgressBarTest, clear);
//    FRIEND_TEST(TextProgressBarTest, update);
//    FRIEND_TEST(TextProgressBarTest, setMessage);
//    FRIEND_TEST(TextProgressBarTest, setStatus);
};

#endif // TEXTPROGRESSBAR_H
