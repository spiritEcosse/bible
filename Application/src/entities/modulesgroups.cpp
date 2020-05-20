#include <QRegularExpression>
#include <QLocale>
#include <QJsonObject>

#include "modulesgroups.h"
#include "global.h"


ModulesGroups::ModulesGroups(const QJsonObject& qJsonModule)
    : m_language { new LocalLanguage { qJsonModule.value("lng").toString() } },
      m_name { qJsonModule.value("fil").toString() } {}

ModulesGroups::ModulesGroups(QString language, QString type, QString region, DBTypes::DBIndex id)
    : m_language {new LocalLanguage {language} },
      m_type {std::move(type)},
      m_region {std::move(region)},
      m_id {id} {}

QString ModulesGroups::nativeLanguageName() const
{
    return m_language->nativeLanguageName();
}

QString ModulesGroups::languageName() const
{
    return m_language->languageToString(m_language->language());
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

void ModulesGroups::fullClean()
{
    cleanName();
}

void ModulesGroups::cleanName()
{
    QRegularExpression re(MODULES_SPLIT_NAME);
    QRegularExpressionMatch match = re.match(m_name);

    if (match.hasMatch()) {
        m_name = match.captured(2).trimmed();
        m_name[0] = m_name[0].toUpper();
    }
}
