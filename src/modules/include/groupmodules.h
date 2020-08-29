#ifndef GROUPMODULES_H
#define GROUPMODULES_H

#include <memory>

#include "dbtypes.h"
#include "locallanguage.h"
#include "module.h"

using namespace DBTypes;

class GroupModules
{
public:
    GroupModules() = default;
    GroupModules(const QJsonObject& qJsonModule);
    GroupModules(QString language, QString name, DBIndex id);
    QString nativeLanguageName() const;
    QString languageName() const;
    QLocale language() const;
    QString region() const;
    std::string regionToStdString() const;
    QString languageCode() const;
    std::string languageCodeToStdString() const;
    QString name();
    uint modulesCount();
    std::string nameToStdString();
    static QString parseName(const QString& name);
    void addModule(const Module& modules);
private:
    friend class tst_GroupModules;
    friend class ModulesGroupsFiller;

    LocalLanguage m_language;
    QString m_name;
    QString m_region;
    DBTypes::DBIndex m_id;
    void cleanName();
    std::vector<Module> m_modules;
};

#endif // GROUPMODULES_H
