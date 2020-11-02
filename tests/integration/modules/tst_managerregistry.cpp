#include <QtTest>
#include <JlCompress.h>
#include "managerregistry.h"

Q_DECLARE_METATYPE(std::vector<modules::Registry>)

namespace modules {

    namespace tests {

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
            const QString fileRegistry { "registry.json" };
            const QString fileRegistryArchive { "registry.zip" };
            QFile fileRegistryInfo { "registry_info.json" };
            const QDir dir;
            void createFileRegistryInfo();
            void createFileRegistry(
                    const QJsonDocument& document = QJsonDocument {},
                    const QString& fileNameRegistry = "registry.json");
            void createFileRegistryArchive(
                    const QJsonDocument& document = QJsonDocument {},
                    const QString& fileNameArchive = "registry.zip",
                    const QString& fileNameRegistry = "registry.json");
            //            QJsonDocument helperGetDocument();
            QJsonDocument helperGetDocument();
            void setQSettings(int value = 0, QString key = "registryVersion");
            const int version = 10;
            const size_t vectorSize = 3;
            std::shared_ptr<db::Db<Registry>> m_db;
            QJsonDocument helperGetInvalidDocument();
            std::vector<Registry> helperGetObjects() const;

        private slots:
            void initTestCase();
            void cleanupTestCase();
            void download_data();
            void download();
            void downloadManagerFailed();
            void removeRegistry();
            void retrieveData_data();
            void retrieveData();
            void hasNewRegistry_data();
            void hasNewRegistry();
            void extractRegistry_data();
            void extractRegistry();
            void transform_data();
            void transform();
            void getDocument_data();
            void getDocument();
            void retrieveVersion();
            void retrieveDataInfo_data();
            void retrieveDataInfo();
            void checkNewVersion_data();
            void checkNewVersion();
            void checkNewVersionDownloadManagerFailed();
            void getVersion_data();
            void getVersion();
            void downloadInfo();
            void downloadRegistry();
            void removeInfo();
            void setVersion_data();
            void setVersion();
            void getVersionFromCache_data();
            void getVersionFromCache();
        };

        void tst_ManagerRegistry::initTestCase()
        {
            dir.mkdir(pathFiles);
            dir.setCurrent(pathFiles);
            dir.mkdir(dirDownload);
        }

        void tst_ManagerRegistry::cleanupTestCase()
        {
            dir.rmdir(dirDownload);
        }

        tst_ManagerRegistry::tst_ManagerRegistry()
            : m_db { db::Db<Registry>::getInstance() } {}

        tst_ManagerRegistry::~tst_ManagerRegistry() {}

        // helpers

        std::vector<Registry> tst_ManagerRegistry::helperGetObjects() const {
            return std::vector<Registry> {vectorSize, {"bGluazE=", "bGluazEx", 1}};
        }

        QJsonDocument tst_ManagerRegistry::helperGetDocument()
        {
            QJsonArray array;
            array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};
            array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};
            array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};

            return QJsonDocument {
                QJsonObject {
                    { "registries",  array },
                    { "downloads", {{"key", "val"}} },
                    { "version", 1 }
                }
            };
        }

        QJsonDocument tst_ManagerRegistry::helperGetInvalidDocument()
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

        void tst_ManagerRegistry::download_data()
        {
            QTest::addColumn<int>("signalRegistryHit");

            QTest::newRow("need signal registry") << 1;
            QTest::newRow("doesn't need signal registry") << 0;
        }

        void tst_ManagerRegistry::download()
        {
            qRegisterMetaType<Registry>("Registry");
            qRegisterMetaType<std::vector<Registry>>("std::vector<Registry>");

            setQSettings();
            createFileRegistryArchive(helperGetDocument());

            QFETCH(int, signalRegistryHit);

            ManagerRegistry managerRegistry;

            QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
            QSignalSpy spyGetDocumentSuccess(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
            QSignalSpy spyRetrieveDataSuccess(&managerRegistry, &ManagerRegistry::retrieveDataSuccess);
            QSignalSpy spyTransformSuccess(&managerRegistry, &ManagerRegistry::transformSuccess);
            QSignalSpy spyRemoveRegistry(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);
            QSignalSpy spyRegistry(managerRegistry.m_modelRegistry.get(), &ModelRegistry::registry);
            QSignalSpy spyUpdateDone(managerRegistry.m_modelRegistry.get(), &ModelRegistry::updateDone);

            if (signalRegistryHit)
            {
                managerRegistry.m_modelRegistry->m_objects = {
                    Registry{
                        QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                        QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                    }
                };
            } else {
                managerRegistry.m_registry.reset(
                            new Registry {
                                QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                                QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                            });
            }

            managerRegistry.download();

            QVERIFY(spyUpdateDone.wait());
            QCOMPARE(spyReadyRead.count(), 1);
            QCOMPARE(spyRegistry.count(), signalRegistryHit);
            QCOMPARE(spyGetDocumentSuccess.count(), 1);
            QCOMPARE(spyRetrieveDataSuccess.count(), 1);
            QCOMPARE(spyTransformSuccess.count(), 1);
            QCOMPARE(spyRemoveRegistry.count(), 1);
            QCOMPARE(spyUpdateDone.count(), 1);
        }

        void tst_ManagerRegistry::downloadManagerFailed()
        {
            qRegisterMetaType<Registry>("Registry");

            setQSettings();
            createFileRegistryArchive(helperGetDocument());

            ManagerRegistry managerRegistry;

            QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
            QSignalSpy spyGetDocumentSuccess(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
            QSignalSpy spyRetrieveDataSuccess(&managerRegistry, &ManagerRegistry::retrieveDataSuccess);
            QSignalSpy spyRemoveRegistry(&managerRegistry, &ManagerRegistry::removeRegistrySuccess);
            QSignalSpy spyUpdateDone(managerRegistry.m_modelRegistry.get(), &ModelRegistry::updateDone);
            QSignalSpy spyRegistry(managerRegistry.m_modelRegistry.get(), &ModelRegistry::registry);
            QSignalSpy spyManagerDownloadFailed(managerRegistry.m_manager.get(), &DownloadManager::failed);

            QString fileRegistryDoesntExist { "registry_doesnt_exist_file.zip" };


            managerRegistry.m_modelRegistry->m_objects = {
                Registry{
                    QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                    QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                }
            };
            managerRegistry.m_registry.reset(
                        new Registry {
                            QString(strUrl + QFileInfo(fileRegistryDoesntExist).absoluteFilePath()).toUtf8().toBase64(),
                            QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                        });

            managerRegistry.download();

            QVERIFY(spyUpdateDone.wait());
            QCOMPARE(spyReadyRead.count(), 1);
            QCOMPARE(spyManagerDownloadFailed.count(), 1);
            QCOMPARE(spyRegistry.count(), 1);
            QCOMPARE(spyGetDocumentSuccess.count(), 1);
            QCOMPARE(spyRetrieveDataSuccess.count(), 1);
            QCOMPARE(spyRemoveRegistry.count(), 1);
            QCOMPARE(spyUpdateDone.count(), 1);
        }

        void tst_ManagerRegistry::downloadRegistry()
        {
            setQSettings();

            createFileRegistryArchive(helperGetDocument());
            ManagerRegistry managerRegistry;

            Registry registry {
                    QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                    QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
            };

            managerRegistry.downloadRegistry(registry);
            QCOMPARE(*managerRegistry.m_registry, registry);
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

            document = helperGetDocument();
            createFileRegistry(document);
            QTest::newRow("success") << fileRegistry << document << 1;
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

            managerRegistry.registryArchive.setFileName(fileRegistryArchive);
            managerRegistry.fileRegistry.setFileName(fileRegistry);

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
            QTest::newRow("success") << fileRegistryArchive;

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

        void tst_ManagerRegistry::transform_data()
        {
            QTest::addColumn<QJsonDocument>("document");
            QTest::addColumn<std::vector<Registry>>("objects");
            QTest::addColumn<bool>("except");

            QTest::newRow("valid data") << helperGetDocument() << helperGetObjects() << false;
            QTest::newRow("not valid data") << helperGetInvalidDocument() << std::vector<Registry>() << true;
        }

        void tst_ManagerRegistry::transform()
        {
            qRegisterMetaType<std::vector<Registry>>("std::vector<Registry>");

            QFETCH(QJsonDocument, document);
            QFETCH(std::vector<Registry>, objects);
            QFETCH(bool, except);

            ManagerRegistry manager;

            manager.transform(document);

            if (!except) {
                QSignalSpy spy(&manager, &ManagerRegistry::transformSuccess);
                manager.transform(document);
                QCOMPARE(spy.count(), 1);
                QList<QVariant> arguments = spy.takeFirst();
                const std::vector<Registry>& registries_actual = arguments[0].value<std::vector<Registry>>();
                QCOMPARE(registries_actual.size(), objects.size());
                QCOMPARE(registries_actual, objects);
            }

        }

        // version

        void tst_ManagerRegistry::checkNewVersion_data()
        {
            setQSettings(0);
            setQSettings(0, "cacheRegistryVersion");

            QTest::addColumn<int>("signalHit");
            QTest::addColumn<int>("signalRegistryHit");

            QTest::newRow("check through download file") << 1 << 0;
            QTest::newRow("check through cache") << 0 << 1;
            QTest::newRow("check through download file : 2") << 1 << 0;
        }

        void tst_ManagerRegistry::checkNewVersion()
        {
            qRegisterMetaType<Registry>("Registry");
            ManagerRegistry managerRegistry;
            createFileRegistryInfo();

            QFETCH(int, signalHit);
            QFETCH(int, signalRegistryHit);

            QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
            QSignalSpy spyRegistry(managerRegistry.m_modelRegistry.get(), &ModelRegistry::registry);
            QSignalSpy spyGetDocumentSuccess(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
            QSignalSpy spyRemoveInfo(&managerRegistry, &ManagerRegistry::removeInfoSuccess);
            QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::newRegistryAvailable);

            if (signalRegistryHit)
            {
                managerRegistry.m_modelRegistry->m_objects = {
                    Registry{
                        QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                        QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                    }
                };
            } else {
                managerRegistry.m_registry.reset(
                            new Registry {
                                QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                                QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                            });
            }

            managerRegistry.checkNewVesion();

            if (signalHit) {
                QVERIFY(spyLast.wait());
                QCOMPARE(QSettings().value("cacheRegistryVersion").toInt(), version);
            } else {
                setQSettings(0, "cacheRegistryVersion");
            }
            QCOMPARE(spyReadyRead.count(), signalHit);
            QCOMPARE(spyRegistry.count(), signalHit & signalRegistryHit);
            QCOMPARE(spyRemoveInfo.count(), signalHit);
            QCOMPARE(spyGetDocumentSuccess.count(), signalHit);
            QCOMPARE(spyLast.count(), 1);

            if (!signalHit) {
                QList<QVariant> arguments = spyLast.takeFirst();
                QCOMPARE(arguments[0].toBool(), true);
                QCOMPARE(arguments[1].toInt(), version);
            }
        }

        void tst_ManagerRegistry::checkNewVersionDownloadManagerFailed()
        {
            setQSettings(0);
            setQSettings(0, "cacheRegistryVersion");

            qRegisterMetaType<Registry>("Registry");
            ManagerRegistry managerRegistry;
            createFileRegistryInfo();

            QSignalSpy spyManagerDownloadFailed(managerRegistry.m_manager.get(), &DownloadManager::failed);
            QSignalSpy spyReadyRead(managerRegistry.m_manager.get(), &DownloadManager::readyRead);
            QSignalSpy spyRegistry(managerRegistry.m_modelRegistry.get(), &ModelRegistry::registry);
            QSignalSpy spyGetDocumentSuccess(&managerRegistry, &ManagerRegistry::getDocumentSuccess);
            QSignalSpy spyRemoveInfo(&managerRegistry, &ManagerRegistry::removeInfoSuccess);
            QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::newRegistryAvailable);

            QString fileRegistryInfoDoesntExist { "registry_info_doesnt_exist_file.json" };

            managerRegistry.m_registry.reset(
                        new Registry {
                            QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                            QString(strUrl + QFileInfo(fileRegistryInfoDoesntExist).absoluteFilePath()).toUtf8().toBase64()
                        });
            managerRegistry.m_modelRegistry->m_objects = {
                Registry{
                    QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
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
            QCOMPARE(spyManagerDownloadFailed.count(), 1);
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

        void tst_ManagerRegistry::downloadInfo()
        {
            ManagerRegistry managerRegistry;

            Registry registry {
                        QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                        QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
            };

            managerRegistry.downloadInfo(registry);
            QCOMPARE(*managerRegistry.m_registry, registry);
        }

        void tst_ManagerRegistry::retrieveDataInfo_data()
        {
            setQSettings(10);

            QTest::addColumn<QJsonDocument>("document");
            QTest::addColumn<int>("version");
            QTest::addColumn<bool>("available");
            QTest::addColumn<int>("signalHit");

            QTest::newRow("has new version")
                    << QJsonDocument {QJsonObject { { "version",  100 } }} << 100 << true << 1;
            QTest::newRow("hasn't new version")
                    << QJsonDocument {QJsonObject { { "version",  3 } }} << 3 << false << 1;
            QTest::newRow("error : value is 0")
                    << QJsonDocument {QJsonObject { { "version",  0 } } } << 0 << false << 0;
        }

        void tst_ManagerRegistry::retrieveDataInfo()
        {
            ManagerRegistry managerRegistry;

            QFETCH(QJsonDocument, document);
            QFETCH(int, version);
            QFETCH(bool, available);
            QFETCH(int, signalHit);

            QSignalSpy spyLast(&managerRegistry, &ManagerRegistry::newRegistryAvailable);

            managerRegistry.retrieveDataInfo(document);

            QCOMPARE(spyLast.count(), signalHit);

            if (signalHit) {
                QList<QVariant> arguments = spyLast.takeFirst();
                QCOMPARE(arguments[0].toBool(), available);
                QCOMPARE(arguments[1].toInt(), version);
            }
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

QTEST_MAIN(modules::tests::tst_ManagerRegistry)

#include "tst_managerregistry.moc"
