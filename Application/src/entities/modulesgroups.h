#ifndef MODULESGROUPS_H
#define MODULESGROUPS_H

#include <memory>

#include "dbtypes.h"
#include "locallanguage.h"

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
private:
    friend class tst_ModulesGroups;

    std::unique_ptr<LocalLanguage> m_language;
    QString m_name;
    DBTypes::DBIndex m_id;
    void cleanName();
};

#endif // MODULESGROUPS_H
