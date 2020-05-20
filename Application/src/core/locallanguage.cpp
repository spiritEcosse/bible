#include "locallanguage.h"
#include <QtDebug>
#include <QVector>

QHash<QString, int> languages {{"av", 0}, {"an", 1}, {"ae", 2}, {"ch", 3}};
QVector<QString> languagesNames {"Avaric", "Aragonese", "Avestan", "Chamorro"};
QVector<QString> nativeLanguagesNames {"Магӏарул мацӏ Maǥarul macʼ", "Aragonés", "Avesta", "Chamoru"};

LocalLanguage::LocalLanguage(const QString& language)
    : QLocale(language)
{
    m_language = language;
}

QString LocalLanguage::languageToString(QLocale::Language language)
{
    QString name;
    if (language == QLocale::C)
    {
        int in = languages.value(m_language);
        if (in != -1) {
            name = languagesNames[in];
        }
    }
    else
    {
        name = QLocale::languageToString(language);
    }

    return name;
}

QString LocalLanguage::nativeLanguageName() const
{
    QString name;

    if (language() == QLocale::C)
    {
        int in = languages.value(m_language);
        if (in != -1) {
            name = nativeLanguagesNames[in];
        }
    }
    else
    {
        name = QLocale::nativeLanguageName();
    }
    return name;
}