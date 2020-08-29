#include <QtTest>
#include <JlCompress.h>
#include "managerregistry.h"

namespace TestManagerRegistry {

class tst_ManagerRegistry : public QObject
{
    Q_OBJECT

public:
    tst_ManagerRegistry();
    ~tst_ManagerRegistry();

private:
    const QString pathFiles { "files" };
    const QString dirDownload { "download" };
    const QString strUrl { "file://" };
    QFile fileRegistry { "registry.json" };
    const QFile fileRegistryArchive { "registry.zip" };
    const QDir dir;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void download_data();
    void download();
    void removeRegistry_data();
    void removeRegistry();
    void retrieveData_data();
    void retrieveData();
    void hasNewRegistry_data();
    void hasNewRegistry();
    void extractRegistry_data();
    void extractRegistry();
};

void tst_ManagerRegistry::cleanupTestCase()
{
    dir.rmdir(dirDownload);
}

void tst_ManagerRegistry::initTestCase()
{
    // Will be called before the first test function is executed.
    dir.mkdir(pathFiles);
    dir.setCurrent(pathFiles);
    dir.mkdir(dirDownload);
}

tst_ManagerRegistry::tst_ManagerRegistry()
{

}

tst_ManagerRegistry::~tst_ManagerRegistry()
{

}

void tst_ManagerRegistry::download_data()
{
    fileRegistry.open(QFile::WriteOnly);
    fileRegistry.write(QJsonDocument {QJsonObject { { "downloads", { } } } }.toJson());
    fileRegistry.close();

    QVERIFY(JlCompress::compressFile(fileRegistryArchive.fileName(), fileRegistry.fileName()));
}

void tst_ManagerRegistry::download()
{
    ManagerRegistry managerRegistry;
    QSignalSpy spyReadyRead(&managerRegistry.manager, &DownloadManager::readyRead);
    QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::retrieveDataResult);
    QSignalSpy spyRemoveRegistry(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);

    QString fileUrl = strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath();
    QByteArray registryQByte(fileUrl.toLocal8Bit());
    managerRegistry.download(registryQByte.toBase64());
    QVERIFY(spyLast.wait());
    QCOMPARE(spyReadyRead.count(), 1);
    QCOMPARE(spyRemoveRegistry.count(), 1);
    QCOMPARE(spyLast.count(), 1);
}

void tst_ManagerRegistry::retrieveData_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QJsonArray>("array");
    QTest::addColumn<QString>("error");

    QJsonArray array ({"key", "value"});

    fileRegistry.open(QFile::WriteOnly);
    fileRegistry.write(QJsonDocument {QJsonObject { { "downloads",  array } } }.toJson());
    fileRegistry.close();

    QTest::newRow("success") << fileRegistry.fileName() << array << "";

    QFile fileRegistryError { "registry_error.json" };
    fileRegistryError.open(QFile::WriteOnly);
    fileRegistryError.close();

    QTest::newRow("error: empty file") << fileRegistryError.fileName() << QJsonArray() << "Something wrong.";

    QFile fileRegistryDoesntExistKey { "registry_doesnt_exist_key.json" };
    fileRegistryDoesntExistKey.open(QFile::WriteOnly);
    fileRegistryDoesntExistKey.write(QJsonDocument {QJsonObject { { "downloads-non",  "" } } }.toJson());
    fileRegistryDoesntExistKey.close();

    QTest::newRow("error : doesnt exist key") << fileRegistryDoesntExistKey.fileName() << QJsonArray() << "Something wrong.";

    QFile fileRegistryDoesntExist { "registry_doesnt_exist.json" };
    QTest::newRow("error : doesnt exist file") << fileRegistryDoesntExist.fileName() << QJsonArray() << "Something wrong.";
}

void tst_ManagerRegistry::retrieveData()
{
    QFETCH(QString, fileName);
    QFETCH(QJsonArray, array);
    QFETCH(QString, error);

    ManagerRegistry managerRegistry;
    QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::retrieveDataResult);
    QSignalSpy spyRemoveRegistry(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);

    managerRegistry.fileRegistry.setFileName(fileName);
    managerRegistry.retrieveData();
    QCOMPARE(spyLast.count(), 1);
    QCOMPARE(spyRemoveRegistry.count(), 1);
    QList<QVariant> arguments = spyLast.takeFirst();
    QCOMPARE(arguments[1].toString(), error);
    QCOMPARE(arguments[0].toJsonArray(), array);
}

void tst_ManagerRegistry::removeRegistry_data()
{
    QFile fileRegistryArchiveD ( QString("%1/%2").arg(dirDownload, fileRegistryArchive.fileName()) );
    fileRegistryArchiveD.open(QFile::WriteOnly);
    fileRegistryArchiveD.close();

    QFile fileRegistryD ( QString("%1/%2").arg(dirDownload, fileRegistry.fileName()) );
    fileRegistryD.open(QFile::WriteOnly);
    fileRegistryD.close();
}

void tst_ManagerRegistry::removeRegistry()
{
    ManagerRegistry managerRegistry;
    QSignalSpy spy(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);
    managerRegistry.registryArchive.setFileName(QString("%1/%2").arg(dirDownload, fileRegistryArchive.fileName()));
    managerRegistry.removeRegistry();

    QCOMPARE(spy.count(), 1);
    QVERIFY(!managerRegistry.registryArchive.exists());
    QVERIFY(!managerRegistry.fileRegistry.exists());
}

void tst_ManagerRegistry::hasNewRegistry_data()
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

void tst_ManagerRegistry::hasNewRegistry()
{
    QFETCH(int, versionInApp);
    QFETCH(int, version);
    QFETCH(bool, availableNewRegistry);

    QSettings settings;
    settings.setValue("registryVersion", versionInApp);

    ManagerRegistry managerRegistry;
    QCOMPARE(managerRegistry.hasNewRegistry(version), availableNewRegistry);
}

void tst_ManagerRegistry::extractRegistry_data()
{
    QTest::addColumn<QString>("fileName");

    fileRegistry.open(QFile::WriteOnly);
    fileRegistry.close();

    QVERIFY(JlCompress::compressFile(fileRegistryArchive.fileName(), fileRegistry.fileName()));
    QTest::newRow("success") << fileRegistryArchive.fileName();

    const QFile fileRegistryArchiveOther { "registry_other.zip" };

    QFile fileRegistryOther { "registry_other.json" };
    fileRegistryOther.open(QFile::WriteOnly);
    fileRegistryOther.close();

    QVERIFY(JlCompress::compressFile(fileRegistryArchiveOther.fileName(), fileRegistryOther.fileName()));
    QTest::newRow("error : other name file") << fileRegistryArchiveOther.fileName();
}

void tst_ManagerRegistry::extractRegistry()
{
    QFETCH(QString, fileName);
    ManagerRegistry managerRegistry;
    managerRegistry.extractRegistry(fileName);
}
}
QTEST_MAIN(TestManagerRegistry::tst_ManagerRegistry)

#include "tst_managerregistry.moc"
