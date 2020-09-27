#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>
#include <QUrl>
#include <QJsonValue>

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
    bool operator==(const Registry& other) const;
private:
    QByteArray m_url;
    short int m_priority;
    QByteArray m_infoUrl;

#ifndef QT_NO_DEBUG_STREAM
    friend QDebug operator<<(QDebug debug, const Registry& registry);
#endif
};

#endif // REGISTRY_H
