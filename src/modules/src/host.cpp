#include <QJsonObject>
#include <QDebug>
#include "host.h"
#include "invaliddata.h"

namespace modules {

    Host::Host()
    {

    }

    Host::Host(const QJsonObject &json)
        : m_alias { json.value("alias").toString().toUtf8() },
          m_path { json.value("path").toString().toUtf8().toBase64() },
          m_priority { json.value("priority").toInt() },
          m_weight { json.value("weight").toInt() }
    {
        if (m_path.isEmpty()) {
            throw(core::InvalidData("Invalid data."));
        }
    }

    Host::Host(const QString &alias, const QByteArray &path, int priority, int weight)
        : m_alias { std::move(alias) },
          m_path { std::move(path) },
          m_priority { std::move(priority) },
          m_weight { std::move(weight) }
    {}

    Host::~Host() {}

    QUrl Host::pathToQUrl() const
    {
        return QUrl::fromEncoded(QByteArray::fromBase64(m_path));
    }

    QString Host::pathToQString() const
    {
        return std::move(QByteArray::fromBase64(m_path));
    }

    QByteArray Host::path() const
    {
        return m_path;
    }

    bool Host::operator==(const Host &other) const
    {
        return m_alias == other.m_alias &&
                m_path == other.m_path &&
                m_priority == other.m_priority &&
                m_weight == other.m_weight;
    }

    #ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug debug, const Host& host)
    {
        return debug << host.m_alias << host.m_path << host.m_priority << host.m_weight;
    }
    #endif

}
