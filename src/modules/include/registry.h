#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>

class QJsonObject;

class Registry
{
public:
    Registry(const QJsonObject& registryJson);
    Registry(QString&& url, short && priority, QString&& infoUrl);

    QString url() const;
    short priority() const;
    QString infoUrl() const;
private:
    QByteArray m_url;
    short int m_priority;
    QByteArray m_infoUrl;
};

#endif // REGISTRY_H
