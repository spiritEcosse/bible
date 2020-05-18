#include <QtTest>
#include "modules.h"

class tst_Modules : public QObject
{
    Q_OBJECT

public:
    tst_Modules();
    ~tst_Modules();

private slots:
    void sizeToInt_data();
    void sizeToInt();
};

tst_Modules::tst_Modules()
{

}

tst_Modules::~tst_Modules()
{

}

void tst_Modules::sizeToInt_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<int>("out");

    QTest::newRow("100.0K") << "100.0K" << 102400;
    QTest::newRow("1K") << "1K" << 1024;
    QTest::newRow("12.32M") << "12.32M" << 12918456;
    QTest::newRow("1m") << "1m" << 1048576;
    QTest::newRow("0.05G") << "0.05G" << 53687091;
    QTest::newRow("1g") << "1g" << 1073741824;
    QTest::newRow("65700") << "65700" << 65700;
}

void tst_Modules::sizeToInt()
{
    QFETCH(QString, in);
    QFETCH(int, out);

    QJsonObject qJsonModule
    {
        {"siz", in},
    };

    Modules modules {qJsonModule};
    QCOMPARE(modules.size(), out);
}

QTEST_APPLESS_MAIN(tst_Modules)

#include "tst_modules.moc"
