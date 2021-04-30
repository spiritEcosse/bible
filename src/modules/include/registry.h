#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QUrl>
#include <QJsonValue>
#include <memory>

class QJsonObject;

namespace modules {

    class Registry
    {
    public:
        Registry();
        Registry(const QJsonObject& registryJson);
        Registry(const QByteArray& url,
                 const QByteArray& infoUrl,
                 const short& priority = 0,
                 const bool& test = false);
        ~Registry();

        QUrl urlToQUrl() const;
        QUrl infoUrlToQUrl() const;
        bool operator==(const Registry& other) const;

        QByteArray m_url;
        QByteArray m_infoUrl;
        short m_priority;
        bool m_test;
        int m_id;

    #ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Registry& registry);
    #endif
    };

    using RegistryShared = decltype(std::shared_ptr<Registry>());
    using RegistryUnique = decltype(std::unique_ptr<Registry>());

}

#endif // REGISTRY_H
