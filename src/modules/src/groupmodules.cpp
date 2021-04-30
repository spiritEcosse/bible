#include <QRegularExpression>
#include <QLocale>
#include <QJsonObject>

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif
#include "modelmodule.h"

#include "groupmodules.h"
#define MODULES_SPLIT_NAME "^(.+)\\.(.+)$"


namespace modules {

    GroupModules::GroupModules() {}

    GroupModules::GroupModules(const QJsonObject& qJsonModule)
        : m_language { qJsonModule.value("lng").toString() }
    {
        m_name = qJsonModule.value("fil").toString();
        m_region = qJsonModule.value("reg").toString();
        cleanName();
    }

    GroupModules::GroupModules(const QString& language,
                               const QString& name,
                               const QString& region,
                               int groupId)
        : m_language { std::move(language) },
          m_name { std::move(name) },
          m_region { std::move(region) },
          m_groupId { groupId }
    {}

    GroupModules::~GroupModules() {}

    QString GroupModules::nativeLanguageName() const
    {
        return m_language.nativeLanguageName();
    }

    QString GroupModules::languageName() const
    {
        return m_language.languageInString();
    }

    QString GroupModules::titleGroup() const
    {
        const QString& language = languageName();
        QString title(language.isEmpty() ? std::move(languageCode()) : std::move(language));
        title += title.isEmpty() ? "" : " - ";
        title += std::move(m_name);
        title += " - " + QString::number(m_countModules);
        return title;
    }

//    QLocale GroupModules::language() const
//    {
//        return m_language;
//    }

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

    QString GroupModules::getLanguageName() const
    {
        return languageName();
    }

    std::string GroupModules::languageCodeToStdString() const
    {
        return languageCode().toStdString();
    }

    QString GroupModules::name() const
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

        return match.hasMatch() ? match.captured(2).trimmed() : "Translations";
    }

    void GroupModules::cleanName()
    {
        m_name = parseName(m_name);
        m_name[0] = m_name[0].toUpper();
    }

    bool GroupModules::operator==(const GroupModules &other) const
    {
        return m_language == other.m_language &&
                m_name == other.m_name &&
                m_region == other.m_region &&
                m_languageName == other.m_languageName &&
                m_groupId == other.m_groupId;
    }

    #ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug debug, const GroupModules& groupModules)
    {
        return debug << groupModules.m_language << groupModules.m_name << groupModules.m_region << groupModules.m_id;
    }
    #endif

}
