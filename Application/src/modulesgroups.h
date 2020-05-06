#pragma once
#include <QString>
#include <QVariant>
#include "dbtypes.h"

class ModulesGroups
{
public:
    ModulesGroups() = default;
    ModulesGroups(QString language, QString type, QString region, DBTypes::DBIndex id);
    QString language() const;
    QString type() const;
    QString region() const;

private:
    QString m_language;
    QString m_type;
    QString m_region;
    DBTypes::DBIndex m_id;

};
