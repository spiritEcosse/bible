#include <QtTest>

#include "groupmodules.h"
#include "locallanguage.h"

namespace modules {

    namespace tests {

        class tst_GroupModules : public QObject
        {
            Q_OBJECT

        public:
            tst_GroupModules();
            ~tst_GroupModules();

        private slots:
            void m_name();
            void m_name_data();
            void language();
            void language_data();
            void m_region();
            void m_region_data();
        };

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

    }

}

QTEST_APPLESS_MAIN(modules::tests::tst_GroupModules)

#include "tst_groupmodules.moc"
