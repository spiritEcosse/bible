#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>

class Registry
{
public:
    Registry(QString&& url, short int&& priority, QString&& infoUrl);

    QString url() const;
    short int priority() const;
    QString infoUrl() const;

private:
    QString m_url;
    short int m_priority;
    QString m_infoUrl;
};

#endif // REGISTRY_H
