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
    QFile fileRegistryInfo { "registry_info.json" };
    const QDir dir;
    ManagerRegistry managerRegistry;

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
    void getDocument_data();
    void getDocument();
    void retrieveVersion_data();
    void retrieveVersion();
    void retrieveDataInfo_data();
    void retrieveDataInfo();
    void checkNewVersion_data();
    void checkNewVersion();
    void getVersion_data();
    void getVersion();
    void downloadInfo_data();
    void downloadInfo();
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
    QSettings settings;
    settings.setValue("registryVersion", 0);

    fileRegistry.open(QFile::WriteOnly);
    fileRegistry.write(QJsonDocument {QJsonObject { { "downloads", {{"key", "val"}} }, {"version", 1} } }.toJson());
    fileRegistry.close();

    QVERIFY(JlCompress::compressFile(fileRegistryArchive.fileName(), fileRegistry.fileName()));
}

void tst_ManagerRegistry::download()
{
    QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
    QSignalSpy spyRemoveRegistry(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);
    QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::retrieveDataSuccess);

    managerRegistry.m_registry.reset(
                new Registry {
                    QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                    1,
                    QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                });

    managerRegistry.download();

    QVERIFY(spyLast.wait());
    QCOMPARE(spyReadyRead.count(), 1);
    QCOMPARE(spyRemoveRegistry.count(), 1);
    QCOMPARE(spyLast.count(), 1);
}

void tst_ManagerRegistry::retrieveData_data()
{
    QSettings settings;
    settings.setValue("registryVersion", 0);

    QTest::addColumn<QJsonDocument>("document");
    QTest::addColumn<QJsonArray>("array");
    QTest::addColumn<int>("signalBit");

    QJsonArray array ({"key", "value"});
    QTest::newRow("success")
            << QJsonDocument {QJsonObject { { "downloads",  array }, { "version", 1 } }} << array << 1;
    QTest::newRow("error : doesn`t exist key")
            << QJsonDocument {QJsonObject { { "downloads-non",  "" } } } << QJsonArray() << 0;
    QTest::newRow("error : value not array")
            << QJsonDocument {QJsonObject { { "downloads",  "" } } } << QJsonArray() << 0;
    QTest::newRow("error: old version")
            << QJsonDocument {QJsonObject { { "downloads",  array }, { "version", 0 } } } << QJsonArray() << 0;
}

void tst_ManagerRegistry::retrieveData()
{
    QFETCH(QJsonDocument, document);
    QFETCH(QJsonArray, array);
    QFETCH(int, signalBit);

    QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::retrieveDataSuccess);

    managerRegistry.retrieveData(document);

    QCOMPARE(spyLast.count(), signalBit);

    if (signalBit) {
        QCOMPARE(spyLast.takeFirst()[0].toJsonArray(), array);
    }
}

void tst_ManagerRegistry::getDocument_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QJsonDocument>("document");
    QTest::addColumn<int>("signalBit");

    QFile fileRegistryError { "registry_empty_file.json" };
    fileRegistryError.open(QFile::WriteOnly);
    fileRegistryError.close();

    QTest::newRow("error: empty file")
            << fileRegistryError.fileName() << QJsonDocument{} << 0;

    QFile fileRegistryDoesntExist { "registry_doesnt_exist_file.json" };
    QTest::newRow("error : doesn`t exist file")
            << fileRegistryDoesntExist.fileName() << QJsonDocument() << 0;

    QJsonDocument document {QJsonObject { } };

    fileRegistry.open(QFile::WriteOnly);
    fileRegistry.write(document.toJson());
    fileRegistry.close();

    QTest::newRow("success") << fileRegistry.fileName() << document << 1;
}

void tst_ManagerRegistry::getDocument()
{
    QFETCH(QString, fileName);
    QFETCH(QJsonDocument, document);
    QFETCH(int, signalBit);

    QSignalSpy spy(&managerRegistry, &ManagerRegistry::getDocumentSuccess);

    managerRegistry.fileRegistry.setFileName(fileName);
    managerRegistry.getDocument(managerRegistry.fileRegistry);

    QCOMPARE(spy.count(), signalBit);

    if (signalBit) {
        QCOMPARE(spy.takeFirst()[0].toJsonDocument(), document);
    }
}

void tst_ManagerRegistry::retrieveVersion_data()
{
    QSettings settings;
    settings.setValue("registryVersion", 0);

    QTest::addColumn<QString>("fileName");

    fileRegistryInfo.open(QFile::WriteOnly);
    fileRegistryInfo.write(QJsonDocument{QJsonObject { {"version", 1} } }.toJson());
    fileRegistryInfo.close();

    QTest::newRow("success") << fileRegistryInfo.fileName();
}

void tst_ManagerRegistry::retrieveVersion()
{
    QFETCH(QString, fileName);

    QSignalSpy spy(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
    QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::newRegistryAvailable);

    managerRegistry.retrieveVersion(fileName);

    QCOMPARE(spyLast.count(), 1);
    QCOMPARE(spy.count(), 1);
}

void tst_ManagerRegistry::retrieveDataInfo_data()
{
    QSettings settings;
    settings.setValue("registryVersion", 10);

    QTest::addColumn<QJsonDocument>("document");
    QTest::addColumn<int>("version");
    QTest::addColumn<bool>("available");
    QTest::addColumn<int>("signalBit");

    QTest::newRow("success")
            << QJsonDocument {QJsonObject { { "version",  100 } }} << 100 << true << 1;
    QTest::newRow("failed")
            << QJsonDocument {QJsonObject { { "version",  3 } }} << 3 << false << 1;
    QTest::newRow("error : value is 0")
            << QJsonDocument {QJsonObject { { "version",  0 } } } << 0 << false << 0;
}

void tst_ManagerRegistry::retrieveDataInfo()
{
    QFETCH(QJsonDocument, document);
    QFETCH(int, version);
    QFETCH(bool, available);
    QFETCH(int, signalBit);

    QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::newRegistryAvailable);

    managerRegistry.retrieveDataInfo(document);

    QCOMPARE(spyLast.count(), signalBit);

    if (signalBit) {
        QList<QVariant> arguments = spyLast.takeFirst();
        QCOMPARE(arguments[0].toBool(), available);
        QCOMPARE(arguments[1].toInt(), version);
    }
}

void tst_ManagerRegistry::checkNewVersion_data()
{
    QSettings settings;
    settings.setValue("registryVersion", 0);

    fileRegistry.open(QFile::WriteOnly);
    fileRegistry.write(QJsonDocument {QJsonObject { { "downloads", {{"key", "val"}} }, {"version", 1} } }.toJson());
    fileRegistry.close();

    QVERIFY(JlCompress::compressFile(fileRegistryArchive.fileName(), fileRegistry.fileName()));
}

void tst_ManagerRegistry::checkNewVersion()
{
    qRegisterMetaType<Registry>("Registry");

    QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
    QSignalSpy spyRemoveInfo(&managerRegistry, &ManagerRegistry::removeInfoSuccess);
    QSignalSpy spyRegistry(managerRegistry.m_modelRegistry.get(), &ModelRegistry::registry);
    QSignalSpy spyGetDocumentSuccess(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
    QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::newRegistryAvailable);

    managerRegistry.m_modelRegistry->m_registries = {
        Registry{
            QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
            1,
            QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
        }
    };

    managerRegistry.checkNewVesion();

    QVERIFY(spyLast.wait());
    QCOMPARE(spyReadyRead.count(), 1);
    QCOMPARE(spyRegistry.count(), 1);
    QCOMPARE(spyRemoveInfo.count(), 1);
    QCOMPARE(spyGetDocumentSuccess.count(), 1);
    QCOMPARE(spyLast.count(), 1);
}

void tst_ManagerRegistry::getVersion_data()
{
    QTest::addColumn<QJsonDocument>("document");
    QTest::addColumn<int>("version");

    QTest::newRow("success")
            << QJsonDocument {QJsonObject { { "version",  100 } }} << 100;
    QTest::newRow("error : doesn`t exist key")
            << QJsonDocument {QJsonObject { { "version-non",  "" } } } << 0;
    QTest::newRow("error : value not int")
            << QJsonDocument {QJsonObject { { "version",  "" } } } << 0;
}

void tst_ManagerRegistry::getVersion()
{
    QFETCH(QJsonDocument, document);
    QFETCH(int, version);

    QCOMPARE(managerRegistry.getVersion(document), version);
}

void tst_ManagerRegistry::downloadInfo_data()
{

}

void tst_ManagerRegistry::downloadInfo()
{
    Registry registry {
        QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
            1,
            QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
        };

    managerRegistry.downloadInfo(registry);
    QCOMPARE(*managerRegistry.m_registry, registry);
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
    managerRegistry.extractRegistry(fileName);
}
}

QTEST_MAIN(TestManagerRegistry::tst_ManagerRegistry)

#include "tst_managerregistry.moc"
