#include "host.h"


Host::Host(QString&& url, short int&& priority, QString&& infoUrl)
    : m_url {url}, m_priority {priority}, m_infoUrl {infoUrl} {}

QString Host::getUrl()
{
    return m_url;
}

short Host::getPriority()
{
    return m_priority;
}

QString Host::getInfoUrl()
{
    return m_infoUrl;
}
