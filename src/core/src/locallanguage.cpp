#include "locallanguage.h"
#include <QVector>

namespace core {

    QHash<QString, QString> languagesNames {
        {"av", "Avaric"},
        {"an", "Aragonese"},
        {"arn", "Mapuche"},
        {"ae", "Avestan"},
        {"ch", "Chamorro"},
        {"ach", "Acoli"},
        {"ada", "Adangme"},
        {"ady", "Adyghe"},
        {"alt", "Southern Altai"},
        {"arc", "Aramaic"},
        {"awa", "Awadhi"},
        {"ay", "Aymara"},
        {"ban", "Balinese"},
        {"bbj", "Ghomala"},
        {"bis", "Bislama"},
        {"bua", "Buriat"},
        {"bum", "Bulu"},
        {"ceb", "Cebuano"},
        {"crh", "Crimean"},
        {"cv", "Chuvash"},
        {"dar", "Dargwa"},
        {"dyu", "Dyula"},
        {"efi", "Efik"},
        {"eo", "Esperanto"},
        {"fat", "Fanti"},
        {"fj", "Fijian"},
        {"gag", "Gagauz"},
        {"gil", "Gilbertese"},
        {"grc", "Greek, Ancient (to 1453)"},
        {"hil", "Hiligaynon"},
        {"ho", "Hiri Motu"},
        {"ht", "Haitian Creole"},
        {"iba", "Iban"},
        {"ilo", "Iloko"},
        {"ibb", "Ibibio"},
        {"kaa", "Kara-Kalpak"},
        {"kac", "Kachin"},
        {"kbd", "Kabardian"},
        {"kha", "Khasi"},
        {"kj", "Kuanyama"},
        {"kmb", "Kimbundu"},
        {"kr", "Kanuri"},
        {"krc", "Karachay-Balkar"},
        {"kru", "Kurukh"},
        {"kum", "Kumyk"},
        {"la", "Latin"},
        {"lam", "Lamba"},
        {"lez", "Lezghian"},
        {"lus", "Lushai"},
        {"maf", "Mafa"},
        {"mai", "Maithili"},
        {"mh", "Marshallese"},
        {"mz", "Mossi"},
        {"ng", "Ndonga"},
        {"nog", "Nogai"},
        {"nv", "Navajo"},
        {"nia", "Nias"},
        {"pag", "Pangasinan"},
        {"pam", "Pampanga"},
        {"rar", "Rarotongan"},
        {"rom", "Romany"},
        {"shu", "Chadian Arabic"},
        {"sm", "Samoan"},
        {"suk", "Sukuma"},
        {"sun", "Sundanese"},
        {"syc", "Classical Syriac"},
        {"tiv", "Tiv"},
        {"tum", "Tumbuka"},
        {"tvl", "Tuvalu"},
        {"tw", "Twi"},
        {"tyv", "Tuvinian"},
        {"umb", "Umbundu"},
        {"war", "Waray"},
        {"xal", "Kalmyk; Oirat"},
        {"yao", "Yao"},
        {"pap", "Papiamento"},
        {"fon", "Fon"}
    };
    QHash<QString, QString> nativeLanguagesNames {
        {"ach", "Lwo"},
        {"ady", "Адыгабзэ"},
        {"ae", "Avesta"},
        {"alt", "Алтай тили"},
        {"am", ""}, // ኣማርኛ
        {"an", "Aragonés"},
        {"arc", ""}, // अवधी
        {"av", "Магӏарул мацӏ Maǥarul macʼ"},
        {"ay", "Aymar aru"},
        {"ban", ""}, // ᬪᬵᬱᬩᬮᬶ; ᬩᬲᬩᬮᬶ; Basa Bali
        {"bo", ""}, // དབུས་སྐད་
        {"bua", "буряад хэлэн"},
        {"bum", "Nkobô Bulu"},
        {"ceb", "Sinugbuanong Binisayâ"},
        {"ch", "Chamoru"},
        {"chr", ""}, // ᏣᎳᎩ, ᏣᎳᎩ ᎦᏬᏂᎯᏍᏗ
        {"crh", "Къырымтатар тили"},
        {"cv", "Чӑвашла"},
        {"dar", "Дарган мез"},
        {"dyu", "Julakan"},
        {"eo", "Esperantaj"},
        {"fat", ""},
        {"fg", "Na vosa vaka-Viti"},
        {"fj", "Na Vosa Vakaviti"},
        {"fon", "Fon gbè"},
        {"gil", "Taetae ni Kiribati"},
        {"grc", "Ἑλληνική"},
        {"hat", "kreyòl ayisyen"},
        {"hil", "Ilonggo"},
        {"ht", "Kreyòl Ayisyen"},
        {"iba", "Jaku Iban"},
        {"ilo", "Pagsasao nga Iloko"},
        {"kaa", "Қарақалпақ тили"},
        {"kac", "Jingpho"},
        {"kbd", "Адыгэбзэ (Къэбэрдейбзэ)"},
        {"kha", "কা কতিয়েন খাশি"},
        {"kj", "Oshikwanyama"},
        {"km", ""}, // ភាសាខ្មែរ
        {"krc", "Къарачай-Малкъар тил"},
        {"kru", "कुड़ुख़"},
        {"kum", "Къумукъ Tил"},
        {"la", "Lingua Latīna"},
        {"lez", "Лезги чӏал"},
        {"lus", "Mizo ṭawng"},
        {"mai", ""}, //मैथिली, মৈথিলী
        {"mh", "Ebon, Kajin M̧ajeļ, Kajin Majõl"},
        {"my", ""},
        {"mz", "Mooré"},
        {"nia", "Li Niha"},
        {"nog", "Ногай тили"},
        {"nv", "Diné bizaad"},
        {"or", ""},
        {"pag", "Salitan Pangasinan"},
        {"pam", "Amánung Kapampangan"},
        {"pap", "Papiamentu"},
        {"rar", "Māori Kūki 'Āirani"},
        {"rom", "romani čhib"},
        {"shu", ""}, // لهجة تشادية
        {"si", ""},
        {"sm", "Gagana faʻa Sāmoa"},
        {"suk", "Kɪsukuma"},
        {"sun", ""}, // ᮘᮞ ᮞᮥᮔ᮪ᮓ / Basa Sunda
        {"ti", ""},
        {"tum", "chiTumbuka"},
        {"tvl", "Te Ggana Tuuvalu"},
        {"tyv", "тыва дыл"},
        {"umb", "Úmbúndú"},
        {"xal", "Хальм́г кел́н"}
    };

    LocalLanguage::LocalLanguage()
        : QLocale() {}

    LocalLanguage::LocalLanguage(const QString& language)
        : QLocale(language),
          m_language { std::move(language) } {}

    QString LocalLanguage::code() const
    {
        return language() == QLocale::C ? m_language : bcp47Name();
    }

    QString LocalLanguage::languageInString() const
    {
        return languagesNames.value(
                    m_language,
                    language() == QLocale::C ?
                        m_language :
                        QLocale::languageToString(language()));
    }

    QString LocalLanguage::nativeLanguageName() const
    {
        return nativeLanguagesNames.value(m_language, QLocale::nativeLanguageName());
    }

}
