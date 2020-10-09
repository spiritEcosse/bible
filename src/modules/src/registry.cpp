#include <QByteArray>
#include <QJsonObject>
#include <QMetaProperty>
#include <stdio.h>
#include <ctype.h>

#include <QDebug>
#include "registry.h"

namespace modules {

    Registry::Registry()
    {

    }

    Registry::Registry(const QJsonObject& registryJson)
        : m_url { registryJson.value("url").toString().toUtf8().toBase64() },
          m_priority { static_cast<short>(registryJson.value("priority").toInt()) },
          m_infoUrl { registryJson.value("info_url").toString().toUtf8().toBase64() }
    {}

    Registry::Registry(const QByteArray& url, const short& priority, const QByteArray& infoUrl)
        : m_url { url },
          m_priority { priority },
          m_infoUrl { infoUrl }
    {
    }

    QByteArray Registry::url() const
    {
        return m_url;
    }

    QUrl Registry::urlToQUrl() const
    {
        return QUrl::fromEncoded(QByteArray::fromBase64(m_url));
    }

    void Registry::setUrl(QByteArray& url)
    {

    }

    short Registry::priority() const
    {
        return m_priority;
    }

    void Registry::setPriority(short &priority)
    {

    }

    QByteArray Registry::infoUrl() const
    {
        return m_infoUrl;
    }

    QUrl Registry::infoUrlToQUrl() const
    {
        return QUrl::fromEncoded(QByteArray::fromBase64(m_infoUrl));;
    }

    void Registry::setInfoUrl(QByteArray& url)
    {

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

}
