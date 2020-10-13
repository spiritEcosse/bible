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
          m_infoUrl { registryJson.value("info_url").toString().toUtf8().toBase64() },
          m_test { registryJson.value("test").toBool() }
    {
    }

    Registry::Registry(const QByteArray& url,
                       const short& priority,
                       const QByteArray& infoUrl,
                       const bool& test)
        : m_url { url },
          m_priority { priority },
          m_infoUrl { infoUrl },
          m_test { test }
    {
    }

    QUrl Registry::urlToQUrl() const
    {
        return QUrl::fromEncoded(QByteArray::fromBase64(m_url));
    }

    QUrl Registry::infoUrlToQUrl() const
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
        return debug << registry.m_url << registry.m_priority << registry.m_infoUrl;
    }
    #endif

}
