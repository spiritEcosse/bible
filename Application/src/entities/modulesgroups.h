#ifndef MODULESGROUPS_H
#define MODULESGROUPS_H

#include <QString>
#include <QVariant>
#include "dbtypes.h"

class ModulesGroups
{
public:
    ModulesGroups(QJsonObject qJsonModule);
    ModulesGroups(QString language, QString type, QString region, DBTypes::DBIndex id);
    QString language() const;
    QString type() const;
    QString region() const;
    QString name();
private:
    QString m_language;
    QString m_name;
    QString m_type;
    QString m_region;
    void cleanName(const QString &name);
    DBTypes::DBIndex m_id;
};

#endif // MODULESGROUPS_H
