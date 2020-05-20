#include <QtTest>
#include "modules.h"

class tst_Modules : public QObject
{
    Q_OBJECT

public:
    tst_Modules();
    ~tst_Modules();

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
//    void testValidator_data();
//    void testValidator();
};

tst_Modules::tst_Modules()
{

}

tst_Modules::~tst_Modules()
{

}

void tst_Modules::m_size_data()
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

void tst_Modules::m_size()
{
    QFETCH(QString, in);
    QFETCH(double, out);

    QJsonObject qJsonModule
    {
        {"siz", in},
    };

    Modules modules {qJsonModule};
    QCOMPARE(modules.m_size, out);
}

void tst_Modules::m_description_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<QString>("out");

    QTest::newRow("Standard") << "New Revised Standard Version" << "New Revised Standard Version";
}

void tst_Modules::m_description()
{
    QFETCH(QString, in);
    QFETCH(QString, out);

    QJsonObject qJsonModule
    {
        {"des", in},
    };

    Modules modules { qJsonModule };
    QCOMPARE(modules.m_description, out);
}

void tst_Modules::m_name_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<QString>("out");

    QTest::newRow("Standard") << "10CD-p.plan" << "10CD-p.plan";
}

void tst_Modules::m_name()
{
    QFETCH(QString, in);
    QFETCH(QString, out);

    QJsonObject qJsonModule
    {
        {"fil", in},
    };

    Modules modules { qJsonModule };
    QCOMPARE(modules.m_name, out);
}

void tst_Modules::m_languageShow_data()
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

void tst_Modules::m_languageShow()
{
    QFETCH(QString, language);
    QFETCH(QString, languageName);
    QFETCH(QString, nativeLanguageName);

    QJsonObject qJsonModule
    {
        {"aln", language}
    };

    LocalLanguage localLanguage { language };
    Modules modules { qJsonModule };
    QCOMPARE(*modules.m_languageShow, localLanguage);
    QCOMPARE(modules.languageNameShow(), languageName);
    QCOMPARE(modules.nativeLanguageNameShow(), nativeLanguageName);
}

void tst_Modules::m_update_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<QDate>("out");
    QTest::newRow("2017-03-31") << "2017-03-31" << QDate(2017, 03, 31);
    QTest::newRow("2016-12-23") << "2016-12-23" << QDate(2016, 12, 23);
}

void tst_Modules::m_update()
{
    QFETCH(QString, in);
    QFETCH(QDate, out);

    QJsonObject qJsonModule
    {
        {"upd", in}
    };

    Modules modules { qJsonModule };
    QCOMPARE(modules.m_update, out);
}

QTEST_APPLESS_MAIN(tst_Modules)

#include "tst_modules.moc"
