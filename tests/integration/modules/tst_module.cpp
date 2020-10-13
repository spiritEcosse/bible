#include <QtTest>
#include "module.h"
#include "locallanguage.h"

namespace modules {

    namespace tests {

        class tst_Module : public QObject {
            Q_OBJECT
        public:
            tst_Module();
            ~tst_Module();

        private slots:
            void m_size_data();
            void m_size();
            void m_description_data();
            void m_description();
            void m_name_data();
            void m_name();
            void m_languageShow_data();
            void m_languageShow();
            void m_update_data();
            void m_update();
            void m_abbreviation_data();
            void m_abbreviation();
            void m_information_data();
            void m_information();
            void m_comment_data();
            void m_comment();
            void m_copyright_data();
            void m_copyright();
            void m_hidden_data();
            void m_hidden();
            void m_defaultDownload_data();
            void m_defaultDownload();
        };

        tst_Module::tst_Module()
        {

        }

        tst_Module::~tst_Module()
        {

        }

        void tst_Module::m_size_data()
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

        void tst_Module::m_size()
        {
            QFETCH(QString, in);
            QFETCH(double, out);

            QJsonObject qJsonModule
            {
                {"siz", in},
            };

            Module Module {qJsonModule};
            QCOMPARE(Module.m_size, out);
        }

        void tst_Module::m_description_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QString>("out");

            QTest::newRow("New Revised Standard Version") << "New Revised Standard Version" << "New Revised Standard Version";
        }

        void tst_Module::m_description()
        {
            QFETCH(QString, in);
            QFETCH(QString, out);

            QJsonObject qJsonModule
            {
                {"des", in},
            };

            Module module { qJsonModule };
            QCOMPARE(module.m_description, out);
        }

        void tst_Module::m_name_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QString>("out");

            QTest::newRow("10CD-p.plan") << "10CD-p.plan" << "10CD-p.plan";
        }

        void tst_Module::m_name()
        {
            QFETCH(QString, in);
            QFETCH(QString, out);

            QJsonObject qJsonModule
            {
                {"fil", in},
            };

            Module Module { qJsonModule };
            QCOMPARE(Module.m_name, out);
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
                {"aln", language}
            };

            core::LocalLanguage localLanguage { language };
            Module Module { qJsonModule };
            QCOMPARE(Module.m_languageShow, localLanguage);
            QCOMPARE(Module.languageNameShow(), languageName);
            QCOMPARE(Module.nativeLanguageNameShow(), nativeLanguageName);
        }

        void tst_Module::m_update_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QDate>("out");
            QTest::newRow("2017-03-31") << "2017-03-31" << QDate(2017, 03, 31);
            QTest::newRow("2016-12-23") << "2016-12-23" << QDate(2016, 12, 23);
        }

        void tst_Module::m_update()
        {
            QFETCH(QString, in);
            QFETCH(QDate, out);

            QJsonObject qJsonModule
            {
                {"upd", in}
            };

            Module Module { qJsonModule };
            QCOMPARE(Module.m_update, out);
        }

        void tst_Module::m_abbreviation_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QString>("out");
            QTest::newRow("10CD-p") << "10CD-p" << "10CD-p";
            QTest::newRow("2000") << "2000" << "2000";
            QTest::newRow("ABP-el+") << "ABP-el+" << "ABP-el+";
            QTest::newRow("ACF'2007") << "ACF'2007" << "ACF'2007";
            QTest::newRow("Apologét") << "Apologét" << "Apologét";
        }

        void tst_Module::m_abbreviation()
        {
            QFETCH(QString, in);
            QFETCH(QString, out);

            QJsonObject qJsonModule
            {
                {"abr", in}
            };

            Module Module { qJsonModule };
            QCOMPARE(Module.m_abbreviation, out);
        }

        void tst_Module::m_information_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QString>("out");
            QTest::newRow("10CD-p") << "10CD-p" << "10CD-p";
            QTest::newRow("2000") << "2000" << "2000";
            QTest::newRow("ABP-el+") << "ABP-el+" << "ABP-el+";
            QTest::newRow("ACF'2007") << "ACF'2007" << "ACF'2007";
            QTest::newRow("Apologét") << "Apologét" << "Apologét";
        }

        void tst_Module::m_information()
        {
            QFETCH(QString, in);
            QFETCH(QString, out);

            QJsonObject qJsonModule
            {
                {"inf", in}
            };

            Module Module { qJsonModule };
            QCOMPARE(Module.m_information, out);
        }

        void tst_Module::m_comment_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QString>("out");
            const char* comment = "(2016-02-18) Code cleaning\n(2015-12-30)";

            QTest::newRow(comment) << comment << comment;
        }

        void tst_Module::m_comment()
        {
            QFETCH(QString, in);
            QFETCH(QString, out);

            QJsonObject qJsonModule
            {
                {"cmt", in}
            };

            Module Module { qJsonModule };
            QCOMPARE(Module.m_comment, out);
        }

        void tst_Module::m_copyright_data()
        {
            QTest::addColumn<QString>("in");
            QTest::addColumn<QString>("out");
            const char* copright {"<br/><h4>Bible Society of South Africa</h4>"};

            QTest::newRow(copright) << copright << copright;
            QTest::newRow("*") << "*" << "*";
        }

        void tst_Module::m_copyright()
        {
            QFETCH(QString, in);
            QFETCH(QString, out);

            QJsonObject qJsonModule
            {
                {"lic", in}
            };

            Module Module { qJsonModule };
            QCOMPARE(Module.m_copyright, out);
        }

        void tst_Module::m_hidden_data()
        {
            QTest::addColumn<bool>("in");
            QTest::addColumn<bool>("out");
            QTest::newRow("1") << true << true;
            QTest::newRow("0") << false << false;
        }

        void tst_Module::m_hidden()
        {
            QFETCH(bool, in);
            QFETCH(bool, out);

            QJsonObject qJsonModule
            {
                {"hid", in}
            };

            Module Module { qJsonModule };
            QCOMPARE(Module.m_hidden, out);
        }

        void tst_Module::m_defaultDownload_data()
        {
            QTest::addColumn<bool>("in");
            QTest::addColumn<bool>("out");
            QTest::newRow("1") << true << true;
            QTest::newRow("0") << false << false;
        }

        void tst_Module::m_defaultDownload()
        {
            QFETCH(bool, in);
            QFETCH(bool, out);

            QJsonObject qJsonModule
            {
                {"def", in}
            };

            Module Module { qJsonModule };
            QCOMPARE(Module.m_defaultDownload, out);
        }

    }
}

QTEST_APPLESS_MAIN(modules::tests::tst_Module)

#include "tst_module.moc"
