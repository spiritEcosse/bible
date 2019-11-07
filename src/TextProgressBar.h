#ifndef TEXTPROGRESSBAR_H
#define TEXTPROGRESSBAR_H

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
};

#endif // TEXTPROGRESSBAR_H
