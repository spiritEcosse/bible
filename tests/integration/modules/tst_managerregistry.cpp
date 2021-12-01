#include <JlCompress.h>
#include "dereferenceiterator.h"
#include "tst_managerregistry.h"

namespace modules {

    namespace tests {

        void tst_ManagerRegistry::initTestCase()
        {
            ModelJsonTest<Registry, ModelRegistry>::initTestCase();
        }

        void tst_ManagerRegistry::cleanupTestCase()
        {
            ModelJsonTest<Registry, ModelRegistry>::cleanupTestCase();
        }

        tst_ManagerRegistry::tst_ManagerRegistry() {}

        tst_ManagerRegistry::~tst_ManagerRegistry() {}

        // helpers

        std::vector<RegistryUnique> tst_ManagerRegistry::helperGetObjectsUnique() const
        {
            return std::vector<RegistryUnique>{};
        }

        std::vector<RegistryShared> tst_ManagerRegistry::helperGetObjects() const
        {
            std::vector<RegistryShared> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_shared<Registry>("bGluazE=", "bGluazEx", 1));
            }
            return objects;
        }

        QJsonDocument tst_ManagerRegistry::helperGetInvalidDocument() const
        {
            QJsonArray array;

            array << QJsonObject {{"url", "link1"}, {"priority", 1}};
            array << QJsonObject {{"priority", 2}, {"info_ufrl", "link22"}};
            array << QJsonObject {{"url", "link3"}, {"priority", 3}, {"info_url", "link33"}};

            return QJsonDocument {
                QJsonObject {
                    { "registries",  array }
                }
            };
        }

        void tst_ManagerRegistry::setQSettings(int value, QString key)
        {
            QSettings settings;
            settings.setValue(key, value);
        }

        void tst_ManagerRegistry::createFileRegistryInfo()
        {
            fileRegistryInfo.open(QFile::WriteOnly);
            fileRegistryInfo.write(QJsonDocument{QJsonObject { {"version", version} } }.toJson());
            fileRegistryInfo.close();
        }

        void tst_ManagerRegistry::createFileRegistry( const QJsonDocument& document, const QString& fileNameRegistry)
        {
            QFile file {fileNameRegistry};
            file.open(QFile::WriteOnly);
            file.write(document.toJson());
            file.close();
        }

        void tst_ManagerRegistry::createFileRegistryArchive(
                const QJsonDocument& document,
                const QString& fileNameArchive,
                const QString& fileNameRegistry)
        {
            QFile file {fileNameArchive};
            createFileRegistry(document, fileNameRegistry);
            QVERIFY(JlCompress::compressFile(fileNameArchive, fileNameRegistry));
        }

        // registry

        void tst_ManagerRegistry::download()
        {
            qRegisterMetaType<std::vector<Registry>>("std::vector<Registry>");
            setQSettings();
            createFileRegistryArchive(ModelJsonTest<Registry, ModelRegistry>::helperGetDocument());

            ManagerRegistry managerRegistry;

            QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
            QSignalSpy spyGetDocumentSuccess(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
            QSignalSpy spyRetrieveDataSuccess(&managerRegistry, &ManagerRegistry::retrieveDataSuccess);
            QSignalSpy spyRemoveRegistry(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);
            QSignalSpy spyTransformSuccess(managerRegistry.m_modelRegistry.get(), &ModelRegistry::transformSuccess);
            QSignalSpy spyTransformSuccessHost(managerRegistry.m_modelHost.get(), &ModelRegistry::transformSuccess);
            QSignalSpy spyUpdateDone(managerRegistry.m_modelRegistry.get(), &ModelRegistry::updateDone);
            QSignalSpy spyUpdateDoneHost(managerRegistry.m_modelHost.get(), &ModelRegistry::updateDone);

            managerRegistry.m_modelRegistry->m_objects.clear();
            managerRegistry.m_modelRegistry->m_objects.push_back(
                std::make_unique<Registry>(
                    QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                    QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                )
            );

            managerRegistry.download();

            QVERIFY(spyUpdateDone.wait());
            QCOMPARE(spyReadyRead.count(), 1);
            QCOMPARE(spyUpdateDoneHost.count(), 1);
            QCOMPARE(spyGetDocumentSuccess.count(), 1);
            QCOMPARE(spyRetrieveDataSuccess.count(), 1);
            QCOMPARE(spyTransformSuccess.count(), 1);
            QCOMPARE(spyTransformSuccessHost.count(), 1);
            QCOMPARE(spyRemoveRegistry.count(), 1);
            QCOMPARE(spyUpdateDone.count(), 1);
        }

        void tst_ManagerRegistry::downloadManagerFailed()
        {
            cleanTable();
            std::vector<RegistryShared> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_shared<Registry>(
                                      QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                                      QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                                      ));
            }
            helperSave(std::move(objects));
            setQSettings();
            createFileRegistryArchive(ModelJsonTest<Registry, ModelRegistry>::helperGetDocument());

            ManagerRegistry managerRegistry;

            QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
            QSignalSpy spyGetDocumentSuccess(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
            QSignalSpy spyRetrieveDataSuccess(&managerRegistry, &ManagerRegistry::retrieveDataSuccess);
            QSignalSpy spyRemoveRegistry(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);
            QSignalSpy spyUpdateDone(managerRegistry.m_modelRegistry.get(), &ModelRegistry::updateDone);
            QSignalSpy spyManagerDownloadFailed(managerRegistry.m_manager.get(), &DownloadManager::failed);

            managerRegistry.m_modelRegistry->m_objects.clear();
            managerRegistry.m_modelRegistry->m_objects.push_back(
                std::make_unique<Registry>(
                    QString(strUrl + QFileInfo("registry_doesnt_exist_file.zip").absoluteFilePath()).toUtf8().toBase64(),
                    QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                )
            );

            managerRegistry.download();

            QVERIFY(spyUpdateDone.wait());
            QCOMPARE(spyReadyRead.count(), 1);
            QCOMPARE(spyManagerDownloadFailed.count(), 1);
            QCOMPARE(spyGetDocumentSuccess.count(), 1);
            QCOMPARE(spyRetrieveDataSuccess.count(), 1);
            QCOMPARE(spyRemoveRegistry.count(), 1);
            QCOMPARE(spyUpdateDone.count(), 1);
        }

        void tst_ManagerRegistry::downloadManagerFailedWithoutRecursion()
        {
            cleanTable();
            helperSave();

            ManagerRegistry managerRegistry;

            QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
            QSignalSpy spyGetDocumentSuccess(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
            QSignalSpy spyRetrieveDataSuccess(&managerRegistry, &ManagerRegistry::retrieveDataSuccess);
            QSignalSpy spyRemoveRegistry(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);
            QSignalSpy spyUpdateDone(managerRegistry.m_modelRegistry.get(), &ModelRegistry::updateDone);
            QSignalSpy spyManagerDownloadFailed(managerRegistry.m_manager.get(), &DownloadManager::failed);
            QSignalSpy spyModelRegistryError(managerRegistry.m_modelRegistry.get(), &ModelRegistry::error);

            managerRegistry.m_modelRegistry->m_objects.clear();
            for ( size_t in = 0; in < vectorSize; in++) {
                managerRegistry.m_modelRegistry->m_objects.push_back(
                    std::make_unique<Registry>(
                        QString(strUrl + QFileInfo("registry_doesnt_exist_file.zip").absoluteFilePath()).toUtf8().toBase64(),
                        QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                    )
                );
            }

            managerRegistry.download();

            QVERIFY(spyModelRegistryError.wait());
            QCOMPARE(spyReadyRead.count(), 0);
            QCOMPARE(spyManagerDownloadFailed.count(), static_cast<int>(vectorSize));
            QCOMPARE(spyGetDocumentSuccess.count(), 0);
            QCOMPARE(spyRetrieveDataSuccess.count(), 0);
            QCOMPARE(spyRemoveRegistry.count(), 0);
            QCOMPARE(spyUpdateDone.count(), 0);
            QCOMPARE(spyModelRegistryError.count(), 1);
        }

        void tst_ManagerRegistry::retrieveData_data()
        {
            setQSettings();

            QTest::addColumn<QJsonDocument>("document");
            QTest::addColumn<int>("signalHit");

            QJsonArray array;
            QTest::newRow("error : doesn`t exist key")
                    << QJsonDocument {QJsonObject { { "downloads-non",  "" } } } << 0;
            QTest::newRow("error : value not array")
                    << QJsonDocument {QJsonObject { { "downloads",  "" } } } << 0;
            QTest::newRow("error: old version")
                    << QJsonDocument {QJsonObject { { "downloads",  array }, { "version", 0 } } } << 0;
            array = {"key", "value"};
            QTest::newRow("success")
                    << QJsonDocument {QJsonObject { { "downloads",  array }, { "version", 1 } }} << 1;
        }

        void tst_ManagerRegistry::retrieveData()
        {
            ManagerRegistry managerRegistry;

            QFETCH(QJsonDocument, document);
            QFETCH(int, signalHit);

            QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::retrieveDataSuccess);

            managerRegistry.retrieveData(document);

            QCOMPARE(spyLast.count(), signalHit);

            if (signalHit) {
                QCOMPARE(spyLast.takeFirst()[0].toJsonDocument(), document);
            }
        }

        void tst_ManagerRegistry::getDocument_data()
        {
            QTest::addColumn<QString>("fileName");
            QTest::addColumn<QJsonDocument>("document");
            QTest::addColumn<int>("signalHit");

            QString fileRegistryError { "registry_empty_file.json" };
            QJsonDocument document {};
            createFileRegistry(document, fileRegistryError);
            QTest::newRow("error: empty file") << fileRegistryError << document << 0;

            QString fileRegistryDoesntExist { "registry_doesnt_exist_file.json" };
            QTest::newRow("error : doesn`t exist file") << fileRegistryDoesntExist << document << 0;

            document = ModelJsonTest<Registry, ModelRegistry>::helperGetDocument();
            createFileRegistry(document);
            QTest::newRow("success") << fileRegistry.fileName() << document << 1;
        }

        void tst_ManagerRegistry::getDocument()
        {
            ManagerRegistry managerRegistry;

            QFETCH(QString, fileName);
            QFETCH(QJsonDocument, document);
            QFETCH(int, signalHit);

            QSignalSpy spy(&managerRegistry, &ManagerRegistry::getDocumentSuccess);

            managerRegistry.fileRegistry.setFileName(fileName);
            managerRegistry.getDocument(managerRegistry.fileRegistry);

            QCOMPARE(spy.count(), signalHit);

            if (signalHit) {
                QCOMPARE(spy.takeFirst()[0].toJsonDocument(), document);
            }
        }

        void tst_ManagerRegistry::removeRegistry()
        {
            createFileRegistryArchive();

            ManagerRegistry managerRegistry;

            QSignalSpy spy(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);

            managerRegistry.registryArchive.setFileName(fileRegistryArchive.fileName());
            managerRegistry.fileRegistry.setFileName(fileRegistry.fileName());

            QVERIFY(managerRegistry.registryArchive.exists());
            QVERIFY(managerRegistry.fileRegistry.exists());

            managerRegistry.removeRegistry();

            QCOMPARE(spy.count(), 1);
            QVERIFY(!managerRegistry.registryArchive.exists());
            QVERIFY(!managerRegistry.fileRegistry.exists());
        }

        void tst_ManagerRegistry::extractRegistry_data()
        {
            QTest::addColumn<QString>("fileName");

            createFileRegistryArchive();
            QTest::newRow("success") << fileRegistryArchive.fileName();

            QString file { "registry_other.zip" };

            createFileRegistryArchive(QJsonDocument {}, file, "registry_other.json");
            QTest::newRow("error : other name file") << file;
        }

        void tst_ManagerRegistry::extractRegistry()
        {
            ManagerRegistry manager;

            QFETCH(QString, fileName);
            manager.extractRegistry(fileName);
        }

        // version

        void tst_ManagerRegistry::checkNewVersion()
        {
            qRegisterMetaType<Registry>("Registry");
            createFileRegistryInfo();

            setQSettings(0);
            setQSettings(0, "cacheRegistryVersion");

            ManagerRegistry managerRegistry;

            QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
            QSignalSpy spyGetDocumentSuccess(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
            QSignalSpy spyRemoveInfo(&managerRegistry, &ManagerRegistry::removeInfoSuccess);
            QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::newRegistryAvailable);

            managerRegistry.m_modelRegistry->m_objects.clear();
            managerRegistry.m_modelRegistry->m_objects.push_back(
                std::make_unique<Registry>(
                    QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                    QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                )
            );

            managerRegistry.checkNewVesion();

            QVERIFY(spyLast.wait());
            QCOMPARE(QSettings().value("cacheRegistryVersion").toInt(), version);
            QCOMPARE(spyReadyRead.count(), 1);
            QCOMPARE(spyRemoveInfo.count(), 1);
            QCOMPARE(spyGetDocumentSuccess.count(), 1);
            QCOMPARE(spyLast.count(), 1);

            QList<QVariant> arguments = spyLast.takeFirst();
            QCOMPARE(arguments[0].toBool(), true);
            QCOMPARE(arguments[1].toInt(), version);
        }

        void tst_ManagerRegistry::removeInfo()
        {
            ManagerRegistry managerRegistry;
            QSignalSpy spyRemoveInfo(&managerRegistry, &ManagerRegistry::removeInfoSuccess);

            createFileRegistryInfo();
            managerRegistry.fileRegistryInfo.setFileName(fileRegistryInfo.fileName());
            QVERIFY(managerRegistry.fileRegistryInfo.exists());

            managerRegistry.removeInfo();
            QVERIFY(!managerRegistry.fileRegistryInfo.exists());
            QCOMPARE(spyRemoveInfo.count(), 1);
        }

        void tst_ManagerRegistry::setVersion_data()
        {
            setQSettings(0, "cacheRegistryVersion");
            QTest::addColumn<bool>("available");
            QTest::addColumn<int>("version");

            QTest::newRow("cache hasn't version") << false << 0;
            QTest::newRow("server has new version, set to cache") << true << 100;
            QTest::newRow("cache has new version") << false << 100;
        }

        void tst_ManagerRegistry::setVersion()
        {
            ManagerRegistry managerRegistry;
            QFETCH(bool, available);
            QFETCH(int, version);

            managerRegistry.setVersion(available, version);

            QCOMPARE(QSettings().value("cacheRegistryVersion").toInt(), version);
        }

        void tst_ManagerRegistry::getVersionFromCache_data()
        {
            QTest::addColumn<int>("version");

            QTest::newRow("version - 10") << 10;
            QTest::newRow("version - 20") << 20;
        }

        void tst_ManagerRegistry::getVersionFromCache()
        {
            QFETCH(int, version);
            setQSettings(version, "cacheRegistryVersion");

            ManagerRegistry managerRegistry;
            QCOMPARE(managerRegistry.getVersion(), version);
        }

        void tst_ManagerRegistry::tryOther_data()
        {
            cleanTable();

            QTest::addColumn<int>("role");
            QTest::addColumn<int>("before_index");
            QTest::addColumn<int>("after_index");

            QTest::newRow("index - 0") << 0 << 0 << 0;
            QTest::newRow("index - 0") << 2 << 3 << 0;
        }

        void tst_ManagerRegistry::tryOther()
        {
            QFETCH(int, role);
            QFETCH(int, before_index);
            QFETCH(int, after_index);

            ManagerRegistry managerRegistry;
            managerRegistry.index = before_index;
            managerRegistry.tryOther(role);
            QCOMPARE(managerRegistry.index, after_index);
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
            ManagerRegistry managerRegistry;

            QFETCH(QJsonDocument, document);
            QFETCH(int, version);

            QCOMPARE(managerRegistry.getVersion(document), version);
        }

        void tst_ManagerRegistry::retrieveDataInfo_data()
        {
            setQSettings(10);

            QTest::addColumn<QJsonDocument>("document");
            QTest::addColumn<int>("version");
            QTest::addColumn<bool>("available");

            QTest::newRow("has new version")
                    << QJsonDocument {QJsonObject { { "version",  100 } }} << 100 << true;
            QTest::newRow("hasn't new version")
                    << QJsonDocument {QJsonObject { { "version",  3 } }} << 3 << false;
            QTest::newRow("error : value is 0")
                    << QJsonDocument {QJsonObject { { "version",  0 } } } << 0 << false;
        }

        void tst_ManagerRegistry::retrieveDataInfo()
        {
            ManagerRegistry managerRegistry;

            QFETCH(QJsonDocument, document);
            QFETCH(int, version);
            QFETCH(bool, available);

            QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::newRegistryAvailable);

            managerRegistry.retrieveDataInfo(document);

            QCOMPARE(spyLast.count(), 1);

            QList<QVariant> arguments = spyLast.takeFirst();
            QCOMPARE(arguments[0].toBool(), available);
            QCOMPARE(arguments[1].toInt(), version);
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
            ManagerRegistry managerRegistry;

            QFETCH(int, versionInApp);
            QFETCH(int, version);
            QFETCH(bool, availableNewRegistry);

            setQSettings(versionInApp);
            QCOMPARE(managerRegistry.hasNewRegistry(version), availableNewRegistry);
        }

        void tst_ManagerRegistry::retrieveVersion()
        {
            createFileRegistryInfo();

            ManagerRegistry managerRegistry;
            QSignalSpy spy(&managerRegistry, &ManagerRegistry::getDocumentSuccess);

            managerRegistry.retrieveVersion(fileRegistryInfo.fileName());

            QCOMPARE(spy.count(), 1);
        }
    }
}

#include "tst_managerregistry.moc"
