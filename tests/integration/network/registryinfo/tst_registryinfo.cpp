#include <QtTest>
#include "registryinfo.h"

class tst_RegistryInfo : public QObject
{
    Q_OBJECT
public:
    tst_RegistryInfo();
    ~tst_RegistryInfo();
private:
    const QString pathFiles { "../../../files" };
    const QString dirDownload = "download";
    const QString strUrl = "http://0.0.0.0:2443/files/";
    QFile file { "registry_info.json" };
    QDir dir;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void download_data();
    void download();
    void newRegistry_data();
    void newRegistry();
};

void tst_RegistryInfo::cleanupTestCase()
{
    dir.rmdir(dirDownload);
}

void tst_RegistryInfo::initTestCase()
{
    // Will be called before the first test function is executed.
    dir.mkdir(pathFiles);
    dir.setCurrent(pathFiles);
    dir.mkdir(dirDownload);
}

tst_RegistryInfo::tst_RegistryInfo()
{

}

tst_RegistryInfo::~tst_RegistryInfo()
{

}

void tst_RegistryInfo::download_data()
{
    QJsonObject jsonObject;
    jsonObject.insert(QString("version"), 1234);
    QJsonDocument document(jsonObject);

    file.open(QFile::WriteOnly);
    file.write(document.toJson());
    file.close();
}

void tst_RegistryInfo::download()
{
    RegistryInfo registry;
    QSignalSpy spy(&registry.manager, &DownloadManager::successfully);
    QSignalSpy spyLast(&registry, &RegistryInfo::newRegistryAvailable);
    QString registryUrl = QString("%1%2").arg(strUrl, file.fileName());
    QByteArray registryQ(registryUrl.toLocal8Bit());

    registry.download(registryQ.toBase64());
    QVERIFY(spyLast.wait());
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyLast.count(), 1);
    QList<QVariant> arguments = spyLast.takeFirst();
    QVERIFY(arguments.at(0).type() == QVariant::Bool);
    QCOMPARE(arguments.at(0).toBool(), true);
}

void tst_RegistryInfo::newRegistry_data()
{
    QTest::addColumn<int>("versionInApp");
    QTest::addColumn<int>("version");
    QTest::addColumn<bool>("availableNewRegistry");
    QTest::newRow("Has new version") << 0 << 1 << true;
    QTest::newRow("Сurrent version") << 1 << 1 << false;
    QTest::newRow("Doesn't have new version") << 2 << 2 << false;
    QTest::newRow("If it is possible, the server version is lower than in the application.")
            << 2 << 0 << false;
}

void tst_RegistryInfo::newRegistry()
{
    QFETCH(int, versionInApp);
    QFETCH(int, version);
    QFETCH(bool, availableNewRegistry);

    QSettings settings;
    settings.setValue("registryVersion", versionInApp);

    RegistryInfo registry;
    QCOMPARE(registry.hasNewRegistry(version), availableNewRegistry);
}

QTEST_MAIN(tst_RegistryInfo)

#include "tst_registryinfo.moc"
