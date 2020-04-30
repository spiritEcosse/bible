#include <QtTest>

#include <QtTest>
#include <QCoreApplication>
#include <quazip/JlCompress.h>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

#include "../../src/DownloadManager.h"
#include "../../src/ModulesModel.h"
#include "../../src/dbmanager.h"
#include "../../src/ModulesGroupModel.h"

typedef QMap<QString, QString> Group;

class DownloadModules : public QObject
{
    Q_OBJECT

public:
    DownloadModules();
    ~DownloadModules();

private:
    const QString dirName = "files";
    QString pathFiles = QString("../../../%1").arg(dirName);
    const QString strUrl = QString("http://0.0.0.0:2443/%1/").arg(dirName);
    QString strUrlTest = QString("%1test").arg(strUrl);
    const QString fileNameRegistry = "registry.json";
    const QString fileNameRegistryInfo = "registry_info.json";
    const QString fileNameRegistryZip = QString("%1.zip").arg(fileNameRegistry);
    const int fileRegistryItems = 5;
    QString tableNameModules = "modules";
    QString tableNameGroupModules = "modules_group";
    QFile fileRegistry;
    QFile fileRegistryInfo;
    QDir dir;
    QSettings settings;

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void singleDownload();
    void multiDownload();
    void updateModules();
    void updateModules_data();
    void newModulesAvailable_data();
    void newModulesAvailable();
    void modulesGroupCorrectTitle_data();
    void modulesGroupCorrectTitle();
    void modulesGroupCorrectSize();
    void modulesGroupCorrectSize_data();
    void modulesGroupRemoveOldRows_data();
    void modulesGroupRemoveOldRows();
    void modulesGroupMakeGroup();
    void modulesGroupMakeGroup_data();
    void modulesNewRows_data();
    void modulesNewRows();
    void modulesModel_data();
    void modulesModel();

};

DownloadModules::DownloadModules()
{
}

DownloadModules::~DownloadModules()
{
}

void DownloadModules::init()
{
}

void DownloadModules::cleanup()
{
}

void DownloadModules::initTestCase()
{
    // Will be called before the first test function is executed.

    dir.mkdir(pathFiles);
    QDir::setCurrent(pathFiles);
    QFile test("test");
    test.open(QFile::ReadOnly);
    test.close();

    connectToDatabase();
    settings.setValue("modulesVersion", 0);
    fileRegistryInfo.setFileName(fileNameRegistryInfo);
    fileRegistry.setFileName(fileNameRegistry);
    QSqlQuery query;
    query.exec("DROP TABLE modules;");
    query.exec("DROP TABLE modules_group;");
}

void DownloadModules::cleanupTestCase()
{
    dir.removeRecursively();
}

void DownloadModules::singleDownload()
{
    DownloadManager manager;
    QSignalSpy spy(&manager, &DownloadManager::successfully);
    manager.append(QUrl(strUrlTest));
    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);
}

void DownloadModules::multiDownload()
{
    QList<QString*> urls {&strUrlTest, &strUrlTest, &strUrlTest, &strUrlTest};

    DownloadManager manager;
    QSignalSpy spy(&manager, &DownloadManager::successfully);
    manager.appendUrls(urls);
    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);
}

void DownloadModules::updateModules_data()
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

void DownloadModules::updateModules()
{
    ModulesGroupModel modulesGroupModel;
    QVERIFY(QSqlDatabase::database().tables().contains("modules_group"));

    QSignalSpy spy(&modulesGroupModel, &ModulesGroupModel::decompressSuccess);
    QSignalSpy spy1(&modulesGroupModel, &ModulesGroupModel::updateTableSuccess);
    QSignalSpy spy2(&modulesGroupModel, &ModulesGroupModel::removeRegistryFileSuccess);
    QSignalSpy spy3(&modulesGroupModel, &ModulesGroupModel::removeOldRowsSuccess);
    QString registryUrl = QString("%1%2").arg(strUrl, fileNameRegistryZip);
    QByteArray registry(registryUrl.toLocal8Bit());
    modulesGroupModel.registryBase64 = registry.toBase64();
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

void DownloadModules::newModulesAvailable_data()
{
    QTest::addColumn<int>("version");
    QTest::addColumn<bool>("newModulesAvailable");
    QTest::addColumn<int>("versionInQSettings");
    QTest::newRow("QSettings has version is 0") << 1 << true << 1;
    QTest::newRow("current version") << 1 << false << 1;
    QTest::newRow("true new version") << 2 << true << 2;
    QTest::newRow("If it is possible, the server version is lower than in the application.")
            << 0 << false << 2;
}

void DownloadModules::newModulesAvailable()
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
    QString registryUrlInfo = QString("%1%2").arg(strUrl, fileRegistryInfo.fileName());
    QByteArray registryInfo(registryUrlInfo.toLocal8Bit());
    modulesGroupModel.registryInfoBase64 = registryInfo.toBase64();
    modulesGroupModel.checkAvailabilityNewModules();

    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).type() == QVariant::Bool);
    QCOMPARE(arguments.at(0).toBool(), newModulesAvailable);
    QCOMPARE(settings.value("modulesVersion").toInt(), versionInQSettings);
}

void DownloadModules::modulesGroupCorrectTitle_data()
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

void DownloadModules::modulesGroupCorrectTitle()
{
    QFETCH(QString, language);
    QFETCH(QString, name);
    QFETCH(QString, region);
    QFETCH(QString, result);

    ModulesGroupModel modulesGroupModel;
    QCOMPARE(modulesGroupModel.correctTitle(name, language, region), result);
}

void DownloadModules::modulesGroupCorrectSize_data()
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

void DownloadModules::modulesGroupCorrectSize()
{
    QFETCH(QString, in);
    QFETCH(int, out);

    ModulesGroupModel modulesGroupModel;
    QCOMPARE(modulesGroupModel.correctSize(in), out);
}

void DownloadModules::modulesGroupRemoveOldRows_data()
{
    QTest::addColumn<int>("rowCount");
    QTest::addColumn<int>("addedRows");
    QTest::newRow("start") << 0 << 10;
    QTest::newRow("middle") << 10 << 3;
    QTest::newRow("end") << 3 << 0;
}

void DownloadModules::modulesGroupRemoveOldRows()
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

void DownloadModules::modulesGroupMakeGroup_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("region");
    QTest::addColumn<Group>("result");

    Group group;
    group["language"] = "en";

    QTest::newRow("name, language exists in qt") << "AUV" << "en" << "" << group;
    group.clear();
    group["region"] = "Papua New Guinea";
    QTest::newRow("name, language not exists in qt and region") << "AUY" << "Auyana" << "Papua New Guinea" << group;

    group.clear();
    group["language"] = "en";
    QTest::newRow("name, language exists in qt and region") << "AUV" << "en" << "India" << group;
    group.clear();

    group["language"] = "av";
    QTest::newRow("name, language not exists in qt") << "AVAR" << "av" << "" << group;

    group.clear();
    group["language"] = "ar";
    QTest::newRow("name, language exists in qt") << "AVDDV" << "ar" << "" << group;

    group.clear();
    group["type"] = "plan";
    QTest::newRow("only name *.plan") << "2016c-p.plan" << "" << "" << group;

    group.clear();
    group["language"] = "ru";
    group["type"] = "plan";
    QTest::newRow("name *.plan, language exists in qt") << "2016c-p.plan" << "ru" << "" << group;

    group.clear();
    group["language"] = "en";
    group["type"] = "commentaries";
    QTest::newRow("name *.commentaries, language exists in qt") << "AB-c.commentaries" << "en" << "" << group;
}

void DownloadModules::modulesGroupMakeGroup()
{
    QFETCH(QString, name);
    QFETCH(QString, language);
    QFETCH(QString, region);
    QFETCH(Group, result);

    ModulesGroupModel modulesGroupModel;
    Group group = modulesGroupModel.makeGroup(name, language, region);
    QCOMPARE(group, result);
}

void DownloadModules::modulesNewRows_data()
{
}

void DownloadModules::modulesNewRows()
{
    QJsonArray jsonArray;
    jsonArray << QJsonObject({
                    qMakePair(QString("fil"), QJsonValue("AUV")),
                    qMakePair(QString("lng"), QJsonValue("en")),
    });
    jsonArray << QJsonObject({
                    qMakePair(QString("fil"), QJsonValue("AUY")),
                    qMakePair(QString("lng"), QJsonValue("Auyana")),
                    qMakePair(QString("reg"), QJsonValue("Papua New Guinea")),
    });
    jsonArray << QJsonObject({
                    qMakePair(QString("fil"), QJsonValue("AUY")),
                    qMakePair(QString("lng"), QJsonValue("Auyana")),
                    qMakePair(QString("reg"), QJsonValue("Papua New Guinea")),
    });

    ModulesGroupModel modulesGroupModel;
    modulesGroupModel.modulesModel->setSort(0, Qt::SortOrder::DescendingOrder);
    modulesGroupModel.modulesModel->select();

    modulesGroupModel.newRows(jsonArray);
    QCOMPARE(modulesGroupModel.rowCount(), 2);

    QSqlRecord record = modulesGroupModel.record(0);
    QCOMPARE(record.value("language").toString(), QString("en"));
    QCOMPARE(record.value("type").toString(), QString(""));
    QCOMPARE(record.value("region").toString(), QString(""));

    QSqlRecord recordRelated = modulesGroupModel.modulesModel->record(2);
    QCOMPARE(recordRelated.value("language").toString(), QString("en"));

    QSqlRecord record1 = modulesGroupModel.record(1);
    QCOMPARE(record1.value("language").toString(), QString(""));
    QCOMPARE(record1.value("type").toString(), QString(""));
    QCOMPARE(record1.value("region").toString(), QString("Papua New Guinea"));

    recordRelated = modulesGroupModel.modulesModel->record(0);
    QCOMPARE(recordRelated.value("language").toString(), QString("Auyana"));
}

void DownloadModules::modulesModel_data()
{
}

void DownloadModules::modulesModel()
{
    ModulesModel modulesModel;
    QVERIFY(QSqlDatabase::database().tables().contains("modules"));
}

QTEST_MAIN(DownloadModules)

#include "tst_downloadmodules.moc"
