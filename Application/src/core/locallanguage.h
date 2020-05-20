#ifndef LOCALLANGUAGE_H
#define LOCALLANGUAGE_H

#include <QLocale>

class LocalLanguage : public QLocale
{
public:
    LocalLanguage(const QString& language);
    QString languageToString(Language language);
    QString nativeLanguageName() const;
private:
    QString m_language;
};

#endif // LOCALLANGUAGE_H
