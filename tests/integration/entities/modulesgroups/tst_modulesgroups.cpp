#include <QtTest>

#include "modulesgroups.h"

class tst_ModulesGroups : public QObject
{
    Q_OBJECT

public:
    tst_ModulesGroups();
    ~tst_ModulesGroups();

private slots:
    void m_name();
    void m_name_data();
    void language();
    void language_data();
};

tst_ModulesGroups::tst_ModulesGroups()
{

}

tst_ModulesGroups::~tst_ModulesGroups()
{

}

void tst_ModulesGroups::m_name_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("result");

    QTest::newRow("name *.plan")
            << "2016c-p.plan" << "Plan";
    QTest::newRow("name *.commentaries")
            << "AB-c.commentaries" << "Commentaries";
    QTest::newRow("name with spaces *.commentaries")
            << " AB-c.commentaries " << "Commentaries";
}

void tst_ModulesGroups::m_name()
{
    QFETCH(QString, name);
    QFETCH(QString, result);

    QJsonObject qJsonModule
    {
        {"fil", name}
    };

    ModulesGroups modulesGroups { qJsonModule };
    QCOMPARE(modulesGroups.m_name, result);
}

void tst_ModulesGroups::language_data()
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

void tst_ModulesGroups::language()
{
    QFETCH(QString, language);
    QFETCH(QString, languageName);
    QFETCH(QString, nativeLanguageName);

    QJsonObject qJsonModule
    {
        {"lng", language}
    };

    LocalLanguage localLanguage { language };
    ModulesGroups modulesGroups { qJsonModule };
    QCOMPARE(*modulesGroups.m_language, localLanguage);
    QCOMPARE(modulesGroups.languageName(), languageName);
    QCOMPARE(modulesGroups.nativeLanguageName(), nativeLanguageName);
}

QTEST_APPLESS_MAIN(tst_ModulesGroups)

#include "tst_modulesgroups.moc"
