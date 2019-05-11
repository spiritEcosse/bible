#ifndef REGISTRY_H
#define REGISTRY_H

#include <QString>

class Registry {
public:
    Registry(
            const QString &alias,
            const quint8 &priority,
            const QString &info_url
            );

    QString url() const;
    quint8 priority() const;
    QString info_url() const;
private:
    QString m_url;
    quint8 m_priority;
    QString m_info_url;
};

#endif // REGISTRY_H
