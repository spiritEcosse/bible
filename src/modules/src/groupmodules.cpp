#include <QRegularExpression>
#include <QLocale>
#include <QJsonObject>
#include <QDebug>

#include "groupmodules.h"
#define MODULES_SPLIT_NAME "^(.+)\\.(.+)$"


GroupModules::GroupModules(const QJsonObject& qJsonModule)
    : m_language { qJsonModule.value("lng").toString() }
{
    m_name = qJsonModule.value("fil").toString();
    m_region = qJsonModule.value("reg").toString();
    cleanName();
}

GroupModules::GroupModules(QString language, QString name, DBTypes::DBIndex id)
    : m_language { language },
      m_name {std::move(name)},
      m_id {id} {}

QString GroupModules::nativeLanguageName() const
{
    return m_language.nativeLanguageName();
}

QString GroupModules::languageName() const
{
    return m_language.languageInString();
}

QLocale GroupModules::language() const
{
    return m_language;
}

QString GroupModules::region() const
{
    return m_region;
}

std::string GroupModules::regionToStdString() const
{
    return m_region.toStdString();
}

QString GroupModules::languageCode() const
{
    return m_language.code();
}

std::string GroupModules::languageCodeToStdString() const
{
    return languageCode().toStdString();
}

QString GroupModules::name()
{
    return m_name;
}

std::string GroupModules::nameToStdString()
{
    return m_name.toStdString();
}

QString GroupModules::parseName(const QString& name)
{
    QRegularExpression re(MODULES_SPLIT_NAME);
    QRegularExpressionMatch match = re.match(name);

    return match.hasMatch() ? match.captured(2).trimmed() : name;
}

void GroupModules::cleanName()
{
    m_name = parseName(m_name);
    m_name[0] = m_name[0].toUpper();
}

void GroupModules::addModule(const Module& modules)
{
    m_modules.push_back(modules);
}

uint GroupModules::modulesCount()
{
    return static_cast<uint>(m_modules.size());
}
