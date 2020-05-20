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

QTEST_APPLESS_MAIN(tst_Modules)

#include "tst_modules.moc"
