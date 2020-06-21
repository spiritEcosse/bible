#ifndef MODULESGROUPS_H
#define MODULESGROUPS_H

#include <memory>

#include "dbtypes.h"
#include "locallanguage.h"
#include "modules.h"

using namespace DBTypes;

class ModulesGroups
{
public:
    ModulesGroups() = default;
    ModulesGroups(const QJsonObject& qJsonModule);
    ModulesGroups(QString language, QString name, DBIndex id);
    QString nativeLanguageName() const;
    QString languageName() const;
    QString name();
    static QString parseName(const QString& name);
    void addModule(const Modules& modules);
private:
    friend class tst_ModulesGroups;
    friend class ModulesGroupsFiller;

    LocalLanguage m_language;
    QString m_name;
    QString m_region;
    DBTypes::DBIndex m_id;
    void cleanName();
    std::vector<Modules> m_modules;
};

#endif // MODULESGROUPS_H
