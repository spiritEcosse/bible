#include <QtTest>
#include <QCoreApplication>
#include "JlCompress.h"

#include "../src/DownloadManager.h"
#include "../src/ModulesModel.h"
#include "../src/dbmanager.h"

class tests : public QObject
{
    Q_OBJECT

public:
    tests();
    ~tests();
private:
    const QString strUrl = "http://0.0.0.0:2443/";
    const QString strUrlTest = QString("%1test").arg(strUrl);
    const QString fileNameRegistry = "registry.json";
    const QString fileNameRegistryInfo = "registry_info.json";
    const QString fileNameRegistryZip = QString("%1.zip").arg(fileNameRegistry);
    const int fileRegistryItems = 10;
    QFile fileRegistry;
    QFile fileRegistryInfo;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void singleDownload();
    void multiDownload();
    void updateModules();
    void updateModules_data();
    void newModulesAvailable_data();
    void newModulesAvailable();
};

tests::tests()
{

}

tests::~tests()
{

}

void tests::initTestCase()
{
    connectToDatabase();
}

void tests::cleanupTestCase()
{
    QSqlQuery query;
    query.exec("DROP TABLE modules;");
}

void tests::singleDownload()
{
    DownloadManager manager;
    QSignalSpy spy(&manager, &DownloadManager::successfully);
    manager.append(QUrl(strUrlTest));
    QVERIFY(spy.wait());
}

void tests::multiDownload()
{
    QStringList urls {strUrlTest, strUrlTest, strUrlTest, strUrlTest};

    DownloadManager manager;
    QSignalSpy spy(&manager, &DownloadManager::successfully);
    manager.append(urls);
    QVERIFY(spy.wait());
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
                    qMakePair(QString("siz"), QJsonValue("100K")),
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

    fileRegistry.setFileName(fileNameRegistry);
    fileRegistry.open(QFile::WriteOnly);
    fileRegistry.write(document.toJson());
    fileRegistry.close();

    if (!JlCompress::compressFile(fileNameRegistryZip, fileRegistry.fileName())) {
        qWarning() << "Could not create: " << fileNameRegistryZip;
    }
    fileRegistry.remove();
}

void tests::updateModules()
{
    ModulesModel modulesModel;
    QVERIFY(QSqlDatabase::database().tables().contains("modules"));

    QSignalSpy spy(&modulesModel, &ModulesModel::updateTableSuccess);
    modulesModel.urlRegistry = QUrl(QString("%1%2").arg(strUrl, fileNameRegistryZip));
    modulesModel.updateModules();
    QVERIFY(spy.wait());

    QCOMPARE(modulesModel.rowCount(), fileRegistryItems);
}

void tests::newModulesAvailable_data()
{
    QTest::addColumn<int>("version");
    QTest::addColumn<bool>("newModulesAvailable");
    QTest::newRow("db not have any version") << 1 << true;
    QTest::newRow("current version") << 0 << false;
    QTest::newRow("true new version") << 2 << true;
}

void tests::newModulesAvailable()
{
    QFETCH(int, version);
    QFETCH(bool, newModulesAvailable);
    QJsonObject jsonObject;
    jsonObject.insert(QString("version"), version);
    QJsonDocument document(jsonObject);

    fileRegistryInfo.setFileName(fileNameRegistryInfo);
    fileRegistryInfo.open(QFile::WriteOnly);
    fileRegistryInfo.write(document.toJson());
    fileRegistryInfo.close();

    ModulesModel modulesModel;
    QSignalSpy spy(&modulesModel, &ModulesModel::availabilityNewModules);
    modulesModel.urlRegistryInfo = QUrl(QString("%1%2").arg(strUrl, fileRegistryInfo.fileName()));
    modulesModel.checkAvailabilityNewModules();

    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).type() == QVariant::Bool);
    QCOMPARE(arguments.at(0), newModulesAvailable);
}

QTEST_MAIN(tests)

#include "tst_tests.moc"
