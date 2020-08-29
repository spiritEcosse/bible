#include "registry.h"


Registry::Registry(QString&& url, short int&& priority, QString&& infoUrl)
    : m_url {url}, m_priority {priority}, m_infoUrl {infoUrl} {}


QString Registry::url() const
{
    return m_url;
}

short Registry::priority() const
{
    return m_priority;
}

QString Registry::infoUrl() const
{
    return m_infoUrl;
}
