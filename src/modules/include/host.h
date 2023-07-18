#ifndef HOST_H
#define HOST_H

#include <QJsonValue>
#include <QString>
#include <QUrl>

class QJsonObject;

namespace modules {

    class Host {
      public:
        Host();
        Host(const QJsonObject &json);
        Host(const QString &alias, const QByteArray &path, int priority = 0, int weight = 0);
        ~Host();

        QString m_alias;
        QByteArray m_path;
        int m_priority;
        int m_weight;
        int m_id;
        QUrl pathToQUrl() const;
        QString pathToQString() const;
        QByteArray path() const;
        bool operator==(const Host &other) const;

#ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Host &host);
#endif
    };

}  // namespace modules

#endif  // HOST_H
