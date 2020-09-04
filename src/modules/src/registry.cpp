#include <QUrl>
#include <QByteArray>
#include <QJsonObject>

#include "registry.h"


Registry::Registry(const QJsonObject& registryJson)
    : m_url { registryJson.value("url").toString().toUtf8().toBase64() },
      m_priority { static_cast<short>(registryJson.value("priority").toInt()) },
      m_infoUrl { registryJson.value("info_url").toString().toUtf8().toBase64() }
{}

Registry::Registry(QString &&url, short &&priority, QString &&infoUrl)
    : m_url { url.toUtf8().toBase64() },
      m_priority { priority },
      m_infoUrl { infoUrl.toUtf8().toBase64() }
{}

QString Registry::url() const
{
    return QString(m_url);
}

short Registry::priority() const
{
    return m_priority;
}

QString Registry::infoUrl() const
{
    return QString(m_infoUrl);
}
