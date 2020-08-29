#include <QtTest>
#include <JlCompress.h>
#include "managergroup.h"
#include "groupmodules.h"
#include "managerregistry.h"

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(QJsonArray)

namespace TestManagerGroup {

class tst_ManagerGroup : public QObject
{
    Q_OBJECT

public:
    tst_ManagerGroup();
    ~tst_ManagerGroup();

private:
    const QString pathFiles { "files" };
    const QString dirDownload = "download";
    QFile fileRegistry { "registry.json" };
    const QFile fileRegistryArchive { "registry.zip" };
    QDir dir;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void addToCollection_data();
    void addToCollection();
    void run_data();
    void run();
    void downloadRegistry_data();
    void downloadRegistry();
};

tst_ManagerGroup::tst_ManagerGroup()
{

}

tst_ManagerGroup::~tst_ManagerGroup()
{

}

void tst_ManagerGroup::initTestCase()
{
    // Will be called before the first test function is executed.
    dir.mkdir(pathFiles);
    dir.setCurrent(pathFiles);
    dir.mkdir(dirDownload);
}

void tst_ManagerGroup::cleanupTestCase()
{
    dir.rmdir(dirDownload);
}

void tst_ManagerGroup::addToCollection_data()
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

void tst_ManagerGroup::addToCollection()
{
    QFETCH(QJsonArray, ar);
    QFETCH(size_t, countMG);
    QFETCH(std::vector<uint>, countM);

    ManagerGroup filler;
    auto mGMap = filler.addToCollection(ar);
    QCOMPARE(mGMap.size(), countMG);

    int index = 0;
    for (auto it = mGMap.begin(); it != mGMap.end(); it++) {
//        qInfo() << it->second.name() << it->second.languageCode() << it->second.region() << it->second.modulesCount(); // WARNING: instead this, make available data if error happened
        QCOMPARE(it->second.modulesCount(), countM[index]);
        index++;
    }
}

void tst_ManagerGroup::run_data()
{
    QTest::addColumn<QJsonArray>("array");

    QJsonArray array { QJsonObject {{"fil", "name"},{"lng", "en"},{"reg", ""}}};
    QTest::newRow("simple case") << array;
}

void tst_ManagerGroup::run()
{
    QFETCH(QJsonArray, array);
    qRegisterMetaType<std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual>>("std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual>"); // WARNING : make this simple
    ManagerGroup filler;
    QSignalSpy spy(&filler, &ManagerGroup::completed);

    filler.run(array);
    QCOMPARE(spy.count(), 1);
}

void tst_ManagerGroup::downloadRegistry_data() {
    fileRegistry.open(QFile::WriteOnly);
    fileRegistry.write(QJsonDocument {QJsonObject { { "downloads", { } } } }.toJson());
    fileRegistry.close();

    QVERIFY(JlCompress::compressFile(fileRegistryArchive.fileName(), fileRegistry.fileName()));
}

void tst_ManagerGroup::downloadRegistry()
{
//    qRegisterMetaType<std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual>>("std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual>"); // WARNING : make this simple
//    ManagerGroup filler;
//    QSignalSpy spy(&*filler.m_registry, &ManagerRegistry::retrieveDataResult);
//    QSignalSpy spyСompleted(&filler, &ManagerGroup::completed);

//    filler.downloadRegistry();
//    QVERIFY(spyСompleted.wait());
//    QCOMPARE(spy.count(), 1);
//    QCOMPARE(spyСompleted.count(), 1);
}

}
QTEST_MAIN(TestManagerGroup::tst_ManagerGroup)

#include "tst_managergroup.moc"
