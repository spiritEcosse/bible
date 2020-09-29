#include <QByteArray>
#include <QJsonObject>

#include <QDebug>
#include "registry.h"

#define quote(x) #x


Registry::Registry()
{

}

Registry::Registry(const QJsonObject& registryJson)
    : m_url { registryJson.value("url").toString().toUtf8().toBase64() },
      m_priority { static_cast<short>(registryJson.value("priority").toInt()) },
      m_infoUrl { registryJson.value("info_url").toString().toUtf8().toBase64() }
{}

Registry::Registry(const char* url, short &&priority, const char* infoUrl)
    : m_url { url },
      m_priority { priority },
      m_infoUrl { infoUrl }
{
}

const QString Registry::tableName()
{
    return QString(quote(Registry));
}

const QStringList Registry::getProperties()
{
}

QUrl Registry::url() const
{
    return QUrl::fromEncoded(QByteArray::fromBase64(m_url));
}

short Registry::priority() const
{
    return m_priority;
}

QUrl Registry::infoUrl() const
{
    return QUrl::fromEncoded(QByteArray::fromBase64(m_infoUrl));;
}

bool Registry::operator==(const Registry &other) const
{
    return m_url == other.m_url &&
            m_infoUrl == other.m_infoUrl &&
            m_priority == other.m_priority;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const Registry& registry)
{
    return debug << registry.url() << registry.priority() << registry.infoUrl();
}
#endif
