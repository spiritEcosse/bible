#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QUrl>
#include <QJsonValue>

class QJsonObject;

class Registry
{
public:
    Registry();
    Registry(const QJsonObject& registryJson);
    Registry(const QByteArray& url, const short& priority, const QByteArray& infoUrl);

    QByteArray url() const;
    QUrl urlToQUrl() const;
    void setUrl(QByteArray& url);
    short priority() const;
    void setPriority(short &priority);
    QByteArray infoUrl() const;
    QUrl infoUrlToQUrl() const;
    void setInfoUrl(QByteArray& url);
    bool operator==(const Registry& other) const;

    QByteArray m_url;
    short m_priority;
    QByteArray m_infoUrl;
    int m_id;

#ifndef QT_NO_DEBUG_STREAM
    friend QDebug operator<<(QDebug debug, const Registry& registry);
#endif
};

#endif // REGISTRY_H
