#include <QByteArray>
#include <QJsonObject>

#include <QDebug>
#include "registry.h"


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
