#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QUrl>
#include <QJsonValue>

class QJsonObject;

namespace modules {

    class Registry
    {
    public:
        Registry();
        Registry(const QJsonObject& registryJson);
        Registry(const QByteArray& url,
                 const short& priority,
                 const QByteArray& infoUrl,
                 const bool& test = false);

        QUrl urlToQUrl() const;
        QUrl infoUrlToQUrl() const;
        bool operator==(const Registry& other) const;

        QByteArray m_url;
        short m_priority;
        QByteArray m_infoUrl;
        bool m_test;
        int m_id;

    #ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Registry& registry);
    #endif
    };

}

#endif // REGISTRY_H
