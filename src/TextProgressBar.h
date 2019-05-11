#ifndef TEXTPROGRESSBAR_H
#define TEXTPROGRESSBAR_H

#include <QString>

class TextProgressBar
{
public:
    void clear();
    void update();
    void setMessage(const QString &message);
    void setStatus(qint64 value, qint64 maximum);

private:
    QString message;
    qint64 value = 0;
    qint64 maximum = -1;
    int iteration = 0;
};

#endif // TEXTPROGRESSBAR_H
