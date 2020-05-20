#ifndef MODULES_H
#define MODULES_H

#include <QString>
#include <QDate>
#include <memory>

#include "locallanguage.h"

class Modules
{
public:
    Modules();
    Modules(const QJsonObject& qJsonModule);
    double size() const;
    QString nativeLanguageNameShow() const;
    QString languageNameShow() const;
private:
    friend class tst_Modules;

    void convertSize(const QString& str);
    void convertUpdate(const QString& update);
    double m_size = 0;
    QString m_name;
    QString m_description;
    QString m_abbreviation;
    QString m_information;
    QDate m_update;
    std::unique_ptr<LocalLanguage> m_languageShow;
};

#endif // MODULES_H
