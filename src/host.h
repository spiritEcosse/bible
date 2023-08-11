#ifndef HOST_H
#define HOST_H

#include <QString>

namespace modules {

    class Host {
    public:
        Host(const QString &alias, const QString &path, const quint8 &priority, const quint8 &weight);

        QString alias() const;
        QString path() const;
        quint8 priority() const;
        quint8 weight() const;
        int m_id;
        QString m_alias;
        QString m_path;
        quint8 m_priority;
        quint8 m_weight;
    };

    using HostShared = decltype(std::shared_ptr<Host>());
}  // namespace modules

#endif  // HOST_H
