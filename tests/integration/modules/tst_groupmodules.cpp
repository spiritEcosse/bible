#include "groupmodules.h"
#include "locallanguage.h"
#include "tst_groupmodules.h"

namespace modules {

    namespace tests {

        tst_GroupModules::tst_GroupModules()
        {

        }

        tst_GroupModules::~tst_GroupModules()
        {

        }

        void tst_GroupModules::m_name_data()
        {
            QTest::addColumn<QString>("name");
            QTest::addColumn<QString>("result");

            QTest::newRow("name") << "name" << "Translations";
            QTest::newRow("name *.plan") << "2016c-p.plan" << "Plan";
            QTest::newRow("name *.commentaries") << "AB-c.commentaries" << "Commentaries";
            QTest::newRow("name with spaces *.commentaries") << " AB-c.commentaries " << "Commentaries";
        }

        void tst_GroupModules::m_name()
        {
            QFETCH(QString, name);
            QFETCH(QString, result);

            GroupModules GroupModules { QJsonObject { {"fil", name} } };
            QCOMPARE(GroupModules.m_name, result);
        }

        void tst_GroupModules::language_data()
        {
            QTest::addColumn<QString>("language");
            QTest::addColumn<QString>("languageName");
            QTest::addColumn<QString>("nativeLanguageName");

            //Doesn't exist in qt
            QTest::newRow("Garifuna - doesn't exist in qt") << "Garifuna" << "Garifuna" << "";
            QTest::newRow("Caluyanon - doesn't exist in qt") << "Caluyanon" << "Caluyanon" << "";

        //    >1. Some ISO 639-1 languages are missing:
        //    >Aragonese (an), Avaric (av), Avestan (ae), Chamorro (ch)
            QTest::newRow("Avaric") << "av" << "Avaric" << "Магӏарул мацӏ Maǥarul macʼ";
            QTest::newRow("Aragonese") << "an" << "Aragonese" << "Aragonés";
            QTest::newRow("Avestan") << "ae" << "Avestan" << "Avesta";
            QTest::newRow("Chamorro") << "ch" << "Chamorro" << "Chamoru";
            QTest::newRow("Mapuche") << "arn" << "Mapuche" << "";
            QTest::newRow("Acoli") << "ach" << "Acoli" << "Lwo";
            QTest::newRow("Adangme") << "ada" << "Adangme" << "";
            QTest::newRow("Adyghe") << "ady" << "Adyghe" << "Адыгабзэ";
            QTest::newRow("Southern Altai") << "alt" << "Southern Altai" << "Алтай тили";
            QTest::newRow("Aramaic") << "arc" << "Aramaic" << "";
            QTest::newRow("Awadhi") << "awa" << "Awadhi" << "";
            QTest::newRow("Aymara") << "ay" << "Aymara" << "Aymar aru";
            QTest::newRow("Balinese") << "ban" << "Balinese" << "";
            QTest::newRow("Ghomala") << "bbj" << "Ghomala" << "";
            QTest::newRow("Bislama") << "bis" << "Bislama" << "";
            QTest::newRow("Buriat") << "bua" << "Buriat" << "буряад хэлэн";
            QTest::newRow("Bulu") << "bum" << "Bulu" << "Nkobô Bulu";
            QTest::newRow("Cebuano") << "ceb" << "Cebuano" << "Sinugbuanong Binisayâ";
            QTest::newRow("Crimean") << "crh" << "Crimean" << "Къырымтатар тили";
            QTest::newRow("Chuvash") << "cv" << "Chuvash" << "Чӑвашла";
            QTest::newRow("Dargwa") << "dar" << "Dargwa" << "Дарган мез";
            QTest::newRow("Dyula") << "dyu" << "Dyula" << "Julakan";
            QTest::newRow("Efik") << "efi" << "Efik" << "";
            QTest::newRow("Esperanto") << "eo" << "Esperanto" << "Esperantaj";
            QTest::newRow("Fanti") << "fat" << "Fanti" << "";
            QTest::newRow("Fijian") << "fj" << "Fijian" << "Na Vosa Vakaviti";
            QTest::newRow("Gagauz") << "gag" << "Gagauz" << "";
            QTest::newRow("Gilbertese") << "gil" << "Gilbertese" << "Taetae ni Kiribati";
            QTest::newRow("Greek, Ancient (to 1453)") << "grc" << "Greek, Ancient (to 1453)" << "Ἑλληνική";
            QTest::newRow("Hiligaynon") << "hil" << "Hiligaynon" << "Ilonggo";
            QTest::newRow("Hiri Motu") << "ho" << "Hiri Motu" << "";
            QTest::newRow("Haitian Creole") << "ht" << "Haitian Creole" << "Kreyòl Ayisyen";
            QTest::newRow("Iban") << "iba" << "Iban" << "Jaku Iban";
            QTest::newRow("Iloko") << "ilo" << "Iloko" << "Pagsasao nga Iloko";
            QTest::newRow("Ibibio") << "ibb" << "Ibibio" << "";
            QTest::newRow("Kara-Kalpak") << "kaa" << "Kara-Kalpak" << "Қарақалпақ тили";
            QTest::newRow("Kachin") << "kac" << "Kachin" << "Jingpho";
            QTest::newRow("Kabardian") << "kbd" << "Kabardian" << "Адыгэбзэ (Къэбэрдейбзэ)";
            QTest::newRow("Khasi") << "kha" << "Khasi" << "কা কতিয়েন খাশি";
            QTest::newRow("Kuanyama") << "kj" << "Kuanyama" << "Oshikwanyama";
            QTest::newRow("Kimbundu") << "kmb" << "Kimbundu" << "";
            QTest::newRow("Kanuri") << "kr" << "Kanuri" << "";
            QTest::newRow("Karachay-Balkar") << "krc" << "Karachay-Balkar" << "Къарачай-Малкъар тил";
            QTest::newRow("Kurukh") << "kru" << "Kurukh" << "कुड़ुख़";
            QTest::newRow("Kumyk") << "kum" << "Kumyk" << "Къумукъ Tил";
            QTest::newRow("Latin") << "la" << "Latin" << "Lingua Latīna";
            QTest::newRow("Lamba") << "lam" << "Lamba" << "";
            QTest::newRow("Lezghian") << "lez" << "Lezghian" << "Лезги чӏал";
            QTest::newRow("Lushai") << "lus" << "Lushai" << "Mizo ṭawng";
            QTest::newRow("Mafa") << "maf" << "Mafa" << "";
            QTest::newRow("Maithili") << "mai" << "Maithili" << "";
            QTest::newRow("Marshallese") << "mh" << "Marshallese" << "Ebon, Kajin M̧ajeļ, Kajin Majõl";
            QTest::newRow("Mossi") << "mz" << "Mossi" << "Mooré";
            QTest::newRow("Ndonga") << "ng" << "Ndonga" << "";
            QTest::newRow("Nogai") << "nog" << "Nogai" << "Ногай тили";
            QTest::newRow("Navajo") << "nv" << "Navajo" << "Diné bizaad";
            QTest::newRow("Nias") << "nia" << "Nias" << "Li Niha";
            QTest::newRow("Pangasinan") << "pag" << "Pangasinan" << "Salitan Pangasinan";
            QTest::newRow("Pampanga") << "pam" << "Pampanga" << "Amánung Kapampangan";
            QTest::newRow("Rarotongan") << "rar" << "Rarotongan" << "Māori Kūki 'Āirani";
            QTest::newRow("Romany") << "rom" << "Romany" << "romani čhib";
            QTest::newRow("Chadian Arabic") << "shu" << "Chadian Arabic" << "";
            QTest::newRow("Samoan") << "sm" << "Samoan" << "Gagana faʻa Sāmoa";
            QTest::newRow("Sukuma") << "suk" << "Sukuma" << "Kɪsukuma";
            QTest::newRow("Sundanese") << "sun" << "Sundanese" << "";
            QTest::newRow("Classical Syriac") << "syc" << "Classical Syriac" << "";
            QTest::newRow("Tiv") << "tiv" << "Tiv" << "";
            QTest::newRow("Tumbuka") << "tum" << "Tumbuka" << "chiTumbuka";
            QTest::newRow("Tuvalu") << "tvl" << "Tuvalu" << "Te Ggana Tuuvalu";
            QTest::newRow("Twi") << "tw" << "Twi" << "";
            QTest::newRow("Tuvinian") << "tyv" << "Tuvinian" << "тыва дыл";
            QTest::newRow("Umbundu") << "umb" << "Umbundu" << "Úmbúndú";
            QTest::newRow("Waray") << "war" << "Waray" << "";
            QTest::newRow("Kalmyk; Oirat") << "xal" << "Kalmyk; Oirat" << "Хальм́г кел́н";
            QTest::newRow("Yao") << "yao" << "Yao" << "";
            QTest::newRow("Papiamento") << "pap" << "Papiamento" << "Papiamentu";
            QTest::newRow("Fon") << "fon" << "Fon" << "Fon gbè";

            QTest::newRow("Chechen") << "ce" << "Chechen" << "";
            QTest::newRow("Bambara") << "bm" << "Bambara" << "bamanakan";
            QTest::newRow("American English") << "en" << "English" << "American English";
            QTest::newRow("Arabic") << "ar" << "Arabic" << "العربية";
            QTest::newRow("Russian") << "ru" << "Russian" << "русский";
            QTest::newRow("Assamese") << "as" << "Assamese" << "অসমীয়া";
            QTest::newRow("Indonesian") << "id" << "Indonesian" << "Bahasa Indonesia";
        }

        void tst_GroupModules::language()
        {
            QFETCH(QString, language);
            QFETCH(QString, languageName);
            QFETCH(QString, nativeLanguageName);

            core::LocalLanguage localLanguage { language };
            GroupModules GroupModules { QJsonObject { {"lng", language} } } ;
            QCOMPARE(GroupModules.m_language, localLanguage);
            QCOMPARE(GroupModules.languageName(), languageName);
            QCOMPARE(GroupModules.nativeLanguageName(), nativeLanguageName);
            QCOMPARE(GroupModules.languageCode(), language);
        }

        void tst_GroupModules::m_region_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QString>("out");

            QTest::newRow("Philippines") << "Philippines" << "Philippines";
            QTest::newRow("Guatemala") << "Guatemala" << "Guatemala";
        }

        void tst_GroupModules::m_region()
        {
            QFETCH(QString, in);
            QFETCH(QString, out);

            GroupModules GroupModules {  QJsonObject { {"reg", in} } };
            QCOMPARE(GroupModules.m_region, out);
        }

        void tst_GroupModules::getLanguageName_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QString>("out");

            QTest::newRow("English") << "en" << "English";
            QTest::newRow("Maithili") << "mai" << "Maithili";
        }

        void tst_GroupModules::getLanguageName()
        {
            QFETCH(QString, in);
            QFETCH(QString, out);

            GroupModules GroupModules {in, "name"};
            QCOMPARE(GroupModules.getLanguageName(), out);
        }

        void tst_GroupModules::setLanguageName_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QString>("out");

            QTest::newRow("English") << "en" << "";
            QTest::newRow("Maithili") << "mai" << "";
        }

        void tst_GroupModules::setLanguageName()
        {
            QFETCH(QString, in);
            QFETCH(QString, out);

            GroupModules GroupModules {in, "name"};
            GroupModules.setLanguageName(in);
            QCOMPARE(GroupModules.m_languageName, out);
        }
    }

}


#include "tst_groupmodules.moc"
