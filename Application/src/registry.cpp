#include "registry.h"

Registry::Registry(
        const QString &url,
        const quint8 &priority,
        const QString &info_url
        )
    : m_url(url), m_priority(priority), m_info_url(info_url)
{}
