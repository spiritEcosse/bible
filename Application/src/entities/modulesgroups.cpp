#include <QRegularExpression>
#include <QLocale>
#include <QJsonObject>

#include "modulesgroups.h"
#include "global.h"


ModulesGroups::ModulesGroups(const QJsonObject& qJsonModule)
    : m_language { new LocalLanguage { qJsonModule.value("lng").toString() } },
      m_name { qJsonModule.value("fil").toString() }
{
    cleanName();
}

ModulesGroups::ModulesGroups(QString language, QString name, DBTypes::DBIndex id)
    : m_language {new LocalLanguage {language} },
      m_name {std::move(name)},
      m_id {id} {}

QString ModulesGroups::nativeLanguageName() const
{
    return m_language->nativeLanguageName();
}

QString ModulesGroups::languageName() const
{
    return m_language->languageToString(m_language->language());
}

QString ModulesGroups::name()
{
    return m_name;
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
