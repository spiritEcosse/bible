#include "modulesgroups.h"
#include <QJsonObject>
#include <QRegularExpression>
#include "global.h"

ModulesGroups::ModulesGroups(QJsonObject qJsonModule)
{
    cleanName(qJsonModule.value("fil").toString());
}

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

QString ModulesGroups::name()
{
    return m_name;
}

QString ModulesGroups::language() const
{
    return m_language;
}

void ModulesGroups::cleanName(const QString &name)
{
    QRegularExpression re(MODULES_SPLIT_NAME);
    QRegularExpressionMatch match = re.match(name);

    if (match.hasMatch()) {
        m_name = match.captured(2);
        m_name[0] = m_name[0].toUpper();
    }
}
