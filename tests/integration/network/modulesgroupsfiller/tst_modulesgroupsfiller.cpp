#include <QtTest>
#include "modulesgroupsfiller.h"

Q_DECLARE_METATYPE(std::string)
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
//    void makeTransform_data();
    void makeTransform();
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
    QTest::newRow("name language region") << "10CD-p.plan" << "en" << "region" << std::string("planenregion");
    QTest::newRow("name language") << "10CD-p.plan" << "en" << "" << std::string("planen");
    QTest::newRow("name") << "name" << "" << "" << std::string("name");
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

void tst_ModulesGroupsFiller::makeTransform()
{
    ModulesGroupsFiller f;
    QJsonArray array;
    QJsonObject obj
    {
        {"fil", "name"},
        {"lng", "en"},
        {"reg", ""},
    };
    array << obj;
    f.makeTransform(array);
}

QTEST_MAIN(tst_ModulesGroupsFiller)

#include "tst_modulesgroupsfiller.moc"
