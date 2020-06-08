#include <QtTest>
#include "modulesgroupsfiller.h"

// add necessary includes here

class tst_ModulesGroupsFiller : public QObject
{
    Q_OBJECT

public:
    tst_ModulesGroupsFiller();
    ~tst_ModulesGroupsFiller();

private slots:
    void hashKey_data();
    void hashKey();

};

tst_ModulesGroupsFiller::tst_ModulesGroupsFiller()
{

}

tst_ModulesGroupsFiller::~tst_ModulesGroupsFiller()
{

}

void tst_ModulesGroupsFiller::hashKey_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("region");
    QTest::addColumn<std::string>("out");
    QTest::newRow("name language region") << "10CD-p.plan" << "en" << "region" << "planenregion";
    QTest::newRow("name language") << "10CD-p.plan" << "en" << "" << "planen";
    QTest::newRow("name") << "name" << "" << "name";
}

void tst_ModulesGroupsFiller::hashKey()
{
    QFETCH(QString, name);
    QFETCH(QString, language);
    QFETCH(QString, region);
    QFETCH(std::string, out);

    QJsonObject obj
    {
        {"fil", name},
        {"lng", language},
        {"reg", region},
    };

    ModulesGroupsFiller f;
    QCOMPARE(f.hashKey(obj), out);
}

QTEST_MAIN(tst_ModulesGroupsFiller)

#include "tst_modulesgroupsfiller.moc"
