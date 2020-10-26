#ifndef LOCALLANGUAGE_H
#define LOCALLANGUAGE_H

#include <QLocale>

namespace core {

    class LocalLanguage : public QLocale
    {
    public:
        LocalLanguage();
        LocalLanguage(const QString& language);
        QString code() const;
        QString languageInString() const;
        QString nativeLanguageName() const;
    private:
        QString m_language;
    };

}

#endif // LOCALLANGUAGE_H
