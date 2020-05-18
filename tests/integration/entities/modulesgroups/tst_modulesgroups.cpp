#include <QtTest>
#include "modulesgroups.h"


class tst_ModulesGroups : public QObject
{
    Q_OBJECT

public:
    tst_ModulesGroups();
    ~tst_ModulesGroups();

private slots:
    void name();
    void name_data();

};

tst_ModulesGroups::tst_ModulesGroups()
{

}

tst_ModulesGroups::~tst_ModulesGroups()
{

}

void tst_ModulesGroups::name_data()
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

void tst_ModulesGroups::name()
{
    QFETCH(QString, name);
    QFETCH(QString, result);

    QJsonObject qJsonModule
    {
        {"fil", name},
    };

    ModulesGroups modulesGroups {qJsonModule};
    QCOMPARE(modulesGroups.name(), result);
}

QTEST_APPLESS_MAIN(tst_ModulesGroups)

#include "tst_modulesgroups.moc"
