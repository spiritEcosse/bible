#include <QtTest>

#include "modulesgroups.h"
#include "locallanguage.h"

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
    void m_region();
    void m_region_data();
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

    QTest::newRow("name *.plan") << "2016c-p.plan" << "Plan";
    QTest::newRow("name *.commentaries") << "AB-c.commentaries" << "Commentaries";
    QTest::newRow("name with spaces *.commentaries") << " AB-c.commentaries " << "Commentaries";
}

void tst_ModulesGroups::m_name()
{
    QFETCH(QString, name);
    QFETCH(QString, result);

    ModulesGroups modulesGroups { QJsonObject { {"fil", name} } };
    QCOMPARE(modulesGroups.m_name, result);
}

void tst_ModulesGroups::language_data()
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

void tst_ModulesGroups::language()
{
    QFETCH(QString, language);
    QFETCH(QString, languageName);
    QFETCH(QString, nativeLanguageName);

    LocalLanguage localLanguage { language };
    ModulesGroups modulesGroups { QJsonObject { {"lng", language} } } ;
    QCOMPARE(modulesGroups.m_language, localLanguage);
    QCOMPARE(modulesGroups.languageName(), languageName);
    QCOMPARE(modulesGroups.nativeLanguageName(), nativeLanguageName);
    QCOMPARE(modulesGroups.languageCode(), language);
}

void tst_ModulesGroups::m_region_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<QString>("out");

    QTest::newRow("Philippines") << "Philippines" << "Philippines";
    QTest::newRow("Guatemala") << "Guatemala" << "Guatemala";
}

void tst_ModulesGroups::m_region()
{
    QFETCH(QString, in);
    QFETCH(QString, out);

    ModulesGroups modulesGroups {  QJsonObject { {"reg", in} } };
    QCOMPARE(modulesGroups.m_region, out);
}


QTEST_APPLESS_MAIN(tst_ModulesGroups)

#include "tst_modulesgroups.moc"
