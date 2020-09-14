#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>
#include <QUrl>

class QJsonObject;

class Registry
{
public:
    Registry();
    Registry(const QJsonObject& registryJson);
    Registry(const char* url, short && priority, const char* infoUrl);

    QUrl url() const;
    short priority() const;
    QUrl infoUrl() const;
private:
    QByteArray m_url;
    short int m_priority;
    QByteArray m_infoUrl;
};

#endif // REGISTRY_H
