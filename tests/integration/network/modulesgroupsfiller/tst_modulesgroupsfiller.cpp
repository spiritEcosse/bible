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
    void fill_data();
    void fill();
    void run_data();
    void run();
    void downloadRegistry();
};

tst_ModulesGroupsFiller::tst_ModulesGroupsFiller()
{

}

tst_ModulesGroupsFiller::~tst_ModulesGroupsFiller()
{

}

void tst_ModulesGroupsFiller::fill_data()
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

void tst_ModulesGroupsFiller::fill()
{
    QFETCH(QJsonArray, ar);
    QFETCH(size_t, countMG);
    QFETCH(std::vector<uint>, countM);

    ModulesGroupsFiller filler;
    auto mGMap = filler.fill(ar);
    QCOMPARE(mGMap.size(), countMG);

    int index = 0;
    for (auto it = mGMap.begin(); it != mGMap.end(); it++) {
//        qInfo() << it->second.name() << it->second.languageCode() << it->second.region() << it->second.modulesCount(); // WARNING: instead this, make available data if error happened
        QCOMPARE(it->second.modulesCount(), countM[index]);
        index++;
    }
}

void tst_ModulesGroupsFiller::run_data()
{
    QTest::addColumn<QJsonArray>("array");

    QJsonArray array { QJsonObject {{"fil", "name"},{"lng", "en"},{"reg", ""}}};
    QTest::newRow("simple case") << array;
}

void tst_ModulesGroupsFiller::run()
{
    QFETCH(QJsonArray, array);
    qRegisterMetaType<std::unordered_map<MGKey, ModulesGroups, MGKeyHash, MGKeyEqual>>("std::unordered_map<MGKey, ModulesGroups, MGKeyHash, MGKeyEqual>"); // WARNING : make this simple
    ModulesGroupsFiller filler;
    QSignalSpy spy(&filler, &ModulesGroupsFiller::completed);

    filler.run(array);
    QCOMPARE(spy.count(), 1);
}

void tst_ModulesGroupsFiller::downloadRegistry()
{
    ModulesGroupsFiller filler;
    QSignalSpy spy(&filler.registry, &Registry::decompressSuccess);

    filler.downloadRegistry();
    QCOMPARE(spy.count(), 1);
}


QTEST_MAIN(tst_ModulesGroupsFiller)

#include "tst_modulesgroupsfiller.moc"
