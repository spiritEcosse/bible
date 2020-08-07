#ifndef HOST_H
#define HOST_H

#include <QString>

class Host
{
public:
    Host(QString&& url, short int&& priority, QString&& infoUrl);

    QString getUrl();
    short int getPriority();
    QString getInfoUrl();

private:
    QString m_url;
    short int m_priority;
    QString m_infoUrl;
};

#endif // HOST_H
