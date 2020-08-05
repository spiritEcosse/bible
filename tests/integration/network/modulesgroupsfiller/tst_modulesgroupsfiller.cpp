#include <QtTest>
#include "modulesgroupsfiller.h"

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(QJsonArray)

class tst_ModulesGroupsFiller : public QObject
{
    Q_OBJECT

public:
    tst_ModulesGroupsFiller();
    ~tst_ModulesGroupsFiller();

private slots:
    void makeTransform_data();
    void makeTransform();
};

tst_ModulesGroupsFiller::tst_ModulesGroupsFiller()
{

}

tst_ModulesGroupsFiller::~tst_ModulesGroupsFiller()
{

}

void tst_ModulesGroupsFiller::makeTransform_data()
{
    QTest::addColumn<QJsonArray>("ar");
    QTest::addColumn<size_t>("countMG");
    QTest::addColumn<std::vector<uint>>("countM");

    QJsonArray array;

    array << QJsonObject {{"fil", "name"},{"lng", "en"},{"reg", ""}};
    QTest::newRow("case: countMG is 1") << array << size_t(1) << std::vector<uint> {1};

    array << QJsonObject {{"fil", "name"},{"lng", "en"},{"reg", ""}};
    QTest::newRow("case: countMG is 1") << array << size_t(1) << std::vector<uint> {2};

    array << QJsonObject {{"fil", "name"},{"lng", "en"},{"reg", "region"}};
    QTest::newRow("case: countMG is 2") << array << size_t(2) << std::vector<uint> {1, 2};

    array << QJsonObject {{"fil", "name"},{"lng", "av"},{"reg", ""}};
    QTest::newRow("case: countMG is 3") << array << size_t(3) << std::vector<uint> {1, 1, 2};

    array << QJsonObject {{"fil", "name"},{"lng", "av"},{"reg", "region"}};
    QTest::newRow("case: countMG is 4") << array << size_t(4) << std::vector<uint> {1, 1, 1, 2};

    array << QJsonObject {{"fil", "name"},{"lng", "av"},{"reg", ""}};
    QTest::newRow("case: countMG is 4") << array << size_t(4) << std::vector<uint> {1, 2, 1, 2};

    array << QJsonObject {{"fil", "name"},{"lng", "av"},{"reg", ""}};
    QTest::newRow("case: countMG is 4") << array << size_t(4) << std::vector<uint> {1, 3, 1, 2};

    array << QJsonObject {{"fil", "name"},{"lng", "av"},{"reg", ""}};
    QTest::newRow("case: countMG is 4") << array << size_t(4) << std::vector<uint> {1, 4, 1, 2};
}

void tst_ModulesGroupsFiller::makeTransform()
{
    QFETCH(QJsonArray, ar);
    QFETCH(size_t, countMG);
    QFETCH(std::vector<uint>, countM);

    ModulesGroupsFiller f;
    auto mGMap = f.makeTransform(ar);
    QCOMPARE(mGMap.size(), countMG);

    int index = 0;
    for (auto it = mGMap.begin(); it != mGMap.end(); it++) {
//        qInfo() << it->second.name() << it->second.languageCode() << it->second.region() << it->second.modulesCount();
        QCOMPARE(it->second.modulesCount(), countM[index]);
        index++;
    }
}

QTEST_MAIN(tst_ModulesGroupsFiller)

#include "tst_modulesgroupsfiller.moc"
