#include "locallanguage.h"
#include "tst_module.h"

Q_DECLARE_METATYPE(modules::Module)

namespace modules {

    namespace tests {

        tst_Module::tst_Module()
        {

        }

        tst_Module::~tst_Module()
        {
        }

        Module tst_Module::helperGetModule()
        {
            return Module (
                        "name",
                        "description",
                        "abbreviation",
                        0,
                        102400,
                        "en",
                        "information",
                        "comment",
                        "copyright",
                        QDate(2017, 03, 31),
                        true,
                        true,
                        false,
                        false);
        }

        void tst_Module::constructor_data()
        {
            QTest::addColumn<QJsonObject>("json_object");
            QTest::addColumn<Module>("object");
            QTest::addColumn<bool>("except");

            QJsonObject data {
                {"fil", "name"},
                {"des", "description"},
                {"abr", "abbreviation"},
                {"siz", "100.0K"},
                {"aln", "en"},
                {"inf", "information"},
                {"cmt", "comment"},
                {"lic", "copyright"},
                {"lic", "copyright"},
                {"upd", "2017-03-31"},
                {"hid", true},
                {"def", true}
            };
            QTest::newRow("valid data") << data << helperGetModule() << false;

            data = {
                {"fil", "name"},
                {"des", "description"},
                {"abr", "abbreviation"}
            };
            QTest::newRow("default fields") << data << Module ("name","description","abbreviation",0,0,"","","","",QDate(),false,false) << false;

            data = {
                {"des", "description"},
                {"abr", "abbreviation"}
            };
            QTest::newRow("invalid data: required m_name.") << data << Module() << true;
        }

        void tst_Module::constructor()
        {
            EntityBase<Module>::constructor();
        }

        void tst_Module::convertSize_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<double>("out");

            QTest::newRow("100.0K") << "100.0K" << double(102400);
            QTest::newRow("1K") << "1K" << double(1024);
            QTest::newRow("12.32M") << "12.32M" << double(12918456.32);
            QTest::newRow("1m") << "1m" << double(1048576);
            QTest::newRow("0.05G") << "0.05G" << double(53687091.2);
            QTest::newRow("1g") << "1g" << double(1073741824);
            QTest::newRow("65700") << "65700" << double(65700);
        }

        void tst_Module::convertSize()
        {
            QFETCH(QString, in);
            QFETCH(double, out);

            Module module = helperGetModule();
            module.convertSize(in);
            QCOMPARE(module.m_size, out);
        }

        void tst_Module::m_languageShow_data()
        {
            QTest::addColumn<QString>("language");
            QTest::addColumn<QString>("languageName");
            QTest::addColumn<QString>("nativeLanguageName");

        //    >1. Some ISO 639-1 languages are missing:
        //    >Aragonese (an), Avaric (av), Avestan (ae), Chamorro (ch)
            QTest::newRow("Avaric") << "av" << "Avaric" << "Магӏарул мацӏ Maǥarul macʼ";
            QTest::newRow("Aragonese") << "an" << "Aragonese" << "Aragonés";
            QTest::newRow("Avestan") << "ae" << "Avestan" << "Avesta";
            QTest::newRow("Chamorro") << "ch" << "Chamorro" << "Chamoru";

            QTest::newRow("Chechen") << "ce" << "Chechen" << "";
            QTest::newRow("Bambara") << "bm" << "Bambara" << "bamanakan";
            QTest::newRow("American English") << "en" << "English" << "American English";
            QTest::newRow("Arabic") << "ar" << "Arabic" << "العربية";
            QTest::newRow("Russian") << "ru" << "Russian" << "русский";
            QTest::newRow("Assamese") << "as" << "Assamese" << "অসমীয়া";
        }

        void tst_Module::m_languageShow()
        {
            QFETCH(QString, language);
            QFETCH(QString, languageName);
            QFETCH(QString, nativeLanguageName);

            QJsonObject qJsonModule
            {
                {"fil", "name"},
                {"des", "description"},
                {"abr", "abbreviation"},
                {"aln", language}
            };

            core::LocalLanguage localLanguage { language };
            Module Module { qJsonModule };
            QCOMPARE(Module.m_languageShow, localLanguage);
            QCOMPARE(Module.languageNameShow(), languageName);
            QCOMPARE(Module.nativeLanguageNameShow(), nativeLanguageName);
        }

    }

}

#include "tst_module.moc"
