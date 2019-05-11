#include "host.h"

Host::Host(
        const QString &alias,
        const QString &path,
        const quint8 &priority,
        const quint8 &weight
        )
    : m_alias(alias), m_path(path), m_priority(priority), m_weight(weight)
{}
