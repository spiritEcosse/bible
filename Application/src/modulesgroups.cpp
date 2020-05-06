#include "modulesgroups.h"

ModulesGroups::ModulesGroups(QString language, QString type, QString region, DBTypes::DBIndex id)
    : m_language {std::move(language)},
      m_type {std::move(type)},
      m_region {std::move(region)},
      m_id {id}
{
}

QString ModulesGroups::type() const
{
    return m_type;
}

QString ModulesGroups::region() const
{
    return m_region;
}

QString ModulesGroups::language() const
{
    return m_language;
}
