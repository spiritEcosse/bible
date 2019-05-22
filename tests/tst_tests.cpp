#include <QtTest>
#include <QCoreApplication>
#include <quazip/JlCompress.h>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

#include "../src/DownloadManager.h"
#include "../src/ModulesModel.h"
#include "../src/dbmanager.h"
#include "../src/ModulesGroupModel.h"

class tests : public QObject
{
    Q_OBJECT

public:
    tests();
    ~tests();
private:
    const QString dirName = "files";
    const QString strUrl = QString("http://0.0.0.0:2443/%1/").arg(dirName);
    const QString strUrlTest = QString("%1test").arg(strUrl);
    const QString fileNameRegistry = "registry.json";
    const QString fileNameRegistryInfo = "registry_info.json";
    const QString fileNameRegistryZip = QString("%1.zip").arg(fileNameRegistry);
    const int fileRegistryItems = 5;
    QFile fileRegistry;
    QFile fileRegistryInfo;
    QDir dir;
    QSettings settings;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void singleDownload();
    void multiDownload();
    void updateModules();
    void updateModules_data();
    void newModulesAvailable_data();
    void newModulesAvailable();
    void modulesGroupCorrectSize_data();
    void modulesGroupCorrectSize();
    void modulesGroupCorrectTitle_data();
    void modulesGroupCorrectTitle();
    void modulesGroupRemoveOldRows_data();
    void modulesGroupRemoveOldRows();
    void modulesModel_data();
    void modulesModel();
};

tests::tests()
{
}

tests::~tests()
{
}

void tests::initTestCase()
{
    dir.mkdir(dirName);
    QDir::setCurrent(dirName);
    connectToDatabase();
    settings.setValue("modulesVersion", 0);
    fileRegistryInfo.setFileName(fileNameRegistryInfo);
    fileRegistry.setFileName(fileNameRegistry);
    QSqlQuery query;
    query.exec("DROP TABLE modules;");
    query.exec("DROP TABLE modules_group;");
}

void tests::cleanupTestCase()
{
    dir.removeRecursively();
}

void tests::singleDownload()
{
    DownloadManager manager;
    QSignalSpy spy(&manager, &DownloadManager::successfully);
    manager.append(QUrl(strUrlTest));
    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);
}

void tests::multiDownload()
{
    QStringList urls {strUrlTest, strUrlTest, strUrlTest, strUrlTest};

    DownloadManager manager;
    QSignalSpy spy(&manager, &DownloadManager::successfully);
    manager.append(urls);
    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);
}

void tests::updateModules_data()
{
    QJsonArray jsonArray;

    auto downloads = QJsonObject(
    {
                    qMakePair(QString("fil"), QJsonValue("name")),
                    qMakePair(QString("des"), QJsonValue("description")),
                    qMakePair(QString("abr"), QJsonValue("abbreviation")),
                    qMakePair(QString("inf"), QJsonValue("information")),
                    qMakePair(QString("lng"), QJsonValue("language")),
                    qMakePair(QString("aln"), QJsonValue("language_show")),
                    qMakePair(QString("upd"), QJsonValue("update")),
                    qMakePair(QString("url"), QJsonValue("urls")),
                    qMakePair(QString("cmt"), QJsonValue("comment")),
                    qMakePair(QString("siz"), QJsonValue("1")),
                    qMakePair(QString("reg"), QJsonValue("region")),
                    qMakePair(QString("def"), QJsonValue().toBool(true)),
                    qMakePair(QString("hid"), QJsonValue().toBool()),
                    qMakePair(QString("lic"), QJsonValue("copyright")),
    });

    for (int i = 0; i < fileRegistryItems; i++) {
        jsonArray << QJsonValue(downloads);
    }

    QJsonObject jsonObject;
    jsonObject.insert(QString("downloads"), QJsonValue(jsonArray));
    QJsonDocument document(jsonObject);

    fileRegistry.open(QFile::WriteOnly);
    fileRegistry.write(document.toJson());
    fileRegistry.close();

    QVERIFY(JlCompress::compressFile(fileNameRegistryZip, fileRegistry.fileName()));
}

void tests::updateModules()
{
    ModulesGroupModel modulesGroupModel;
    QVERIFY(QSqlDatabase::database().tables().contains("modules_group"));

    QSignalSpy spy(&modulesGroupModel, &ModulesGroupModel::decompressSuccess);
    QSignalSpy spy1(&modulesGroupModel, &ModulesGroupModel::updateTableSuccess);
    QSignalSpy spy2(&modulesGroupModel, &ModulesGroupModel::removeRegistryFileSuccess);
    QSignalSpy spy3(&modulesGroupModel, &ModulesGroupModel::removeOldRowsSuccess);
    modulesGroupModel.urlRegistry = QUrl(QString("%1%2").arg(strUrl, fileNameRegistryZip));
    modulesGroupModel.updateModules();

    QVERIFY(spy2.wait());
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 1);
    QCOMPARE(spy3.count(), 1);

    ModulesGroupModel modulesGroupModelNew;
    QCOMPARE(modulesGroupModelNew.rowCount(), fileRegistryItems);
    modulesGroupModelNew.setCountOldRows();
    modulesGroupModelNew.removeOldRows();
}

void tests::newModulesAvailable_data()
{
    QTest::addColumn<int>("version");
    QTest::addColumn<bool>("newModulesAvailable");
    QTest::addColumn<int>("versionInQSettings");
    QTest::newRow("db not have any version") << 1 << true << 1;
    QTest::newRow("current version") << 0 << false << 1;
    QTest::newRow("true new version") << 2 << true << 2;
    QTest::newRow("If it is possible, the server version is lower than in the application.")
            << 0 << false << 2;
}

void tests::newModulesAvailable()
{
    QFETCH(int, version);
    QFETCH(bool, newModulesAvailable);
    QFETCH(int, versionInQSettings);
    QJsonObject jsonObject;
    jsonObject.insert(QString("version"), version);
    QJsonDocument document(jsonObject);

    fileRegistryInfo.open(QFile::WriteOnly);
    fileRegistryInfo.write(document.toJson());
    fileRegistryInfo.close();

    ModulesGroupModel modulesGroupModel;
    QSignalSpy spy(&modulesGroupModel, &ModulesGroupModel::availabilityNewModules);
    modulesGroupModel.urlRegistryInfo = QUrl(QString("%1%2").arg(strUrl, fileRegistryInfo.fileName()));
    modulesGroupModel.checkAvailabilityNewModules();

    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).type() == QVariant::Bool);
    QCOMPARE(arguments.at(0).toBool(), newModulesAvailable);
    QCOMPARE(settings.value("modulesVersion").toInt(), versionInQSettings);
}

void tests::modulesGroupCorrectSize_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("result");
    QTest::newRow("100K") << "100K" << 102400;
    QTest::newRow("100.0K") << "100.0K" << 102400;
    QTest::newRow("1k") << "1k" << 1024;
    QTest::newRow("12.32M") << "12.32M" << 12918456;
    QTest::newRow("1m") << "1m" << 1048576;
    QTest::newRow("0.05G") << "0.05G" << 53687091;
    QTest::newRow("1g") << "1g" << 1073741824;
    QTest::newRow("65700") << "65700" << 65700;
}

void tests::modulesGroupCorrectSize()
{
    QFETCH(QString, input);
    QFETCH(int, result);

    ModulesGroupModel modulesGroupModel;
    QCOMPARE(modulesGroupModel.correctSize(input), result);
}

void tests::modulesGroupCorrectTitle_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("region");
    QTest::addColumn<QString>("result");
    QTest::newRow("name, language exists in qt")
            << "AUV"            << "en" << "" << "American English";
    QTest::newRow("name, language not exists in qt and region")
            << "AUY"            << "Auyana" << "Papua New Guinea" << "Papua New Guinea";
    QTest::newRow("name, language exists in qt and region")
            << "AUV"            << "en" << "India" << "American English";
    QTest::newRow("name, language not exists in qt")
            << "AVAR"           << "av" << "" << "Av";
    QTest::newRow("name, language exists in qt")
            << "AVDDV"          << "ar" << "" << "العربية";
    QTest::newRow("only name *.plan")
            << "2016c-p.plan"   << "" << "" << "Plan";
    QTest::newRow("name *.plan, language exists in qt")
            << "2016c-p.plan"   << "ru" << "" << "Plan, русский";
    QTest::newRow("name *.commentaries, language exists in qt")
            << "AB-c.commentaries" << "en" << "" << "Commentaries, American English";
}

void tests::modulesGroupCorrectTitle()
{
    QFETCH(QString, language);
    QFETCH(QString, name);
    QFETCH(QString, region);
    QFETCH(QString, result);

    ModulesGroupModel modulesGroupModel;
    QCOMPARE(modulesGroupModel.correctTitle(name, language, region), result);
}

void tests::modulesGroupRemoveOldRows_data()
{
    QTest::addColumn<int>("rowCount");
    QTest::addColumn<int>("addedRows");
    QTest::newRow("start") << 0 << 10;
    QTest::newRow("middle") << 10 << 3;
    QTest::newRow("end") << 3 << 0;
}

void tests::modulesGroupRemoveOldRows()
{
    QFETCH(int, rowCount);
    QFETCH(int, addedRows);

    ModulesGroupModel modulesGroupModel;
    QCOMPARE(modulesGroupModel.rowCount(), rowCount);

    modulesGroupModel.setCountOldRows();

    for (int i = 0; i < addedRows; i++) {
        QSqlRecord newRecord = modulesGroupModel.record();
        newRecord.setValue("title", "title");
        modulesGroupModel.insertRecord(-1, newRecord);
    }

    modulesGroupModel.removeOldRows();

    if (!modulesGroupModel.submitAll()) {
        qWarning() << "Failed to add new row: " << modulesGroupModel.lastError().text();
    }
    QCOMPARE(modulesGroupModel.rowCount(), addedRows);
}

void tests::modulesModel_data()
{
}

void tests::modulesModel()
{
    ModulesModel modulesModel;
    QVERIFY(QSqlDatabase::database().tables().contains("modules"));
}

QTEST_MAIN(tests)

#include "tst_tests.moc"
