#ifndef MODULES_H
#define MODULES_H

#include <QString>
#include <memory>

#include "locallanguage.h"

class QJsonObject;

class Modules
{
public:
    Modules();
    Modules(const QJsonObject& qJsonModule);
    double size() const;
private:
    friend class tst_Modules;

    void convertSize(const QString& str);
    double m_size = 0;
    QString m_name;
    QString m_description;
    QString m_abbreviation;
    QString m_information;
    std::unique_ptr<LocalLanguage> m_language;
};

#endif // MODULES_H
