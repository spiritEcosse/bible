#include <QRegularExpression>
#include <QLocale>
#include <QJsonObject>
#include <QDebug>

#include "modulesgroups.h"
#include "global.h"


ModulesGroups::ModulesGroups(const QJsonObject& qJsonModule)
    : m_language { qJsonModule.value("lng").toString() }
{
    qJsonModule.value("fil").toString().swap(m_name);
    qJsonModule.value("reg").toString().swap(m_region);
    cleanName();
}

ModulesGroups::ModulesGroups(QString language, QString name, DBTypes::DBIndex id)
    : m_language { language },
      m_name {std::move(name)},
      m_id {id} {}

QString ModulesGroups::nativeLanguageName() const
{
    return m_language.nativeLanguageName();
}

QString ModulesGroups::languageName() const
{
    return m_language.languageInString(m_language.language());
}

QString ModulesGroups::name()
{
    return m_name;
}

QString ModulesGroups::parseName(const QString& name)
{
    QRegularExpression re(MODULES_SPLIT_NAME);
    QRegularExpressionMatch match = re.match(name);

    return match.hasMatch() ? match.captured(2).trimmed() : name;
}

void ModulesGroups::cleanName()
{
    m_name = parseName(m_name);
    m_name[0] = m_name[0].toUpper();
}

void ModulesGroups::addModule(const Modules& modules)
{
    m_modules.push_back(modules);
}
