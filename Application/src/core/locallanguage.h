#ifndef LOCALLANGUAGE_H
#define LOCALLANGUAGE_H

#include <QLocale>

class LocalLanguage : public QLocale
{
public:
    LocalLanguage() = default;
    LocalLanguage(const QString& language);
    QString languageInString(Language language) const;
    QString nativeLanguageName() const;
private:
    QString m_language;
};

#endif // LOCALLANGUAGE_H
