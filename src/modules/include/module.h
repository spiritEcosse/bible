#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QDate>

#include "locallanguage.h"

class QJsonObject;

class Module
{
public:
    Module() = default;
    Module(const QJsonObject& qJsonModule);
    double size() const;
    QString nativeLanguageNameShow() const;
    QString languageNameShow() const;
private:
    friend class tst_Module;
    friend class ModulesGroupsFiller;

    void convertSize(const QString& str);
    void convertUpdate(const QString& update);
    double m_size = 0;
    QString m_name;
    QString m_description;
    QString m_abbreviation;
    QString m_information;
    QString m_comment;
    QString m_copyright;
    QDate m_update;
    bool m_hidden;
    bool m_defaultDownload;
    LocalLanguage m_languageShow;
};

#endif // MODULE_H
