#ifndef MODULESGROUPSFORM_H
#define MODULESGROUPSFORM_H
#include <QString>

class ModulesGroupsForm
{
public:
    ModulesGroupsForm(const QString& language, const QString& type, const QString& region);
    ~ModulesGroupsForm();
private:
    QString m_language;
    QString m_type;
    QString m_region;
};

#endif // MODULESGROUPSFORM_H
