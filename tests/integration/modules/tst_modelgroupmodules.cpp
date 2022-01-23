#include <JlCompress.h>
#include "dereferenceiterator.h"
#include "tst_modelgroupmodules.h"

Q_DECLARE_METATYPE(std::vector<modules::GroupModulesShared>)

namespace modules {

    namespace tests {

        tst_ModelGroupModules::tst_ModelGroupModules() {}

        tst_ModelGroupModules::~tst_ModelGroupModules() {}

        void tst_ModelGroupModules::initTestCase()
        {
            ModelJsonTest<GroupModules, ModelGroupModules>::initTestCase();
        }

        void tst_ModelGroupModules::cleanupTestCase()
        {
            ModelJsonTest<GroupModules, ModelGroupModules>::cleanupTestCase();
        }

        //helpers

        void tst_ModelGroupModules::setQSettings(int value, QString key)
        {
            QSettings settings;
            settings.setValue(key, value);
        }

        std::vector<GroupModulesShared> tst_ModelGroupModules::helperGetObjects() const
        {
            std::vector<GroupModulesShared> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_shared<GroupModules>("en", "Translations", "region", in + 1));
            }
            return objects;
        }

        std::vector<GroupModulesUnique> tst_ModelGroupModules::helperGetObjectsUnique() const
        {
            std::vector<GroupModulesUnique> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_unique<GroupModules>(
                                      "en",
                                      "Translations",
                                      "region",
                                      in + 1));
            }
            return objects;
        }

        void tst_ModelGroupModules::helperSaveStatic()
        {
            tst_ModelGroupModules tst_modelGroup;
            tst_modelGroup.initDb();
            tst_modelGroup.helperSave();
        }

//      tests
        void tst_ModelGroupModules::contructor_data()
        {
            QTest::addColumn<bool>("available");
            QTest::addColumn<int>("version");

            QTest::newRow("available new version is true") << true << 11;
            QTest::newRow("available new version is false") << false << 0;
        }

        void tst_ModelGroupModules::contructor()
        {
            QFETCH(bool, available);
            QFETCH(int, version);
            setQSettings(version, "cacheRegistryVersion");

            ModelGroupModules modelGroupModules;
            QCOMPARE(modelGroupModules.m_newVersionAvailable, available);
        }

        void tst_ModelGroupModules::newVersionAvailable()
        {
            ModelGroupModules model;
            QCOMPARE(model.newVersionAvailable(), false);
        }

        void tst_ModelGroupModules::updateCompleted()
        {
            ModelGroupModules model;
            QCOMPARE(model.updateCompleted(), false);
        }

        void tst_ModelGroupModules::update()
        {
            ModelJsonTest<GroupModules, ModelGroupModules>::update();
        }

        void tst_ModelGroupModules::downloadRegistry_data() {
            QSettings settings;
            settings.setValue("registryVersion", 0);

            fileRegistry.open(QFile::WriteOnly);
            fileRegistry.write(
                        QJsonDocument {
                            QJsonObject {
                                {
                                    "downloads",
                                    QJsonArray {
                                        QJsonObject {
                                            {"fil", "100EJ-p.plan"},
                                        },
                                        QJsonObject {
                                            {"fil", "10CD-p.plan"},
                                        },
                                        QJsonObject {
                                            {"fil", "2000.dictionary"},
                                            {"lng", "en"},
                                        }
                                    },
                                },
                                {"version", 1}
                            }
                        }.toJson());
            fileRegistry.close();

            QVERIFY(JlCompress::compressFile(fileRegistryArchive.fileName(), fileRegistry.fileName()));
        }

        void tst_ModelGroupModules::downloadRegistry()
        {
            ModelGroupModules modelGroupModules;
            QSignalSpy spy(&modelGroupModules, &ModelGroupModules::updateDone);
            QSignalSpy spyChangeNewVersionAvailable(&modelGroupModules, &ModelGroupModules::changeNewVersionAvailable);
            QSignalSpy spyChangeUpdateCompleted(&modelGroupModules, &ModelGroupModules::changeUpdateCompleted);
            QSignalSpy spyModulesUpdateCompleted(modelGroupModules.m_modelModule.get(), &ModelModule::updateDone);

            modelGroupModules.m_managerRegistry->m_modelRegistry->m_objects.clear();
            modelGroupModules.m_managerRegistry->m_modelRegistry->m_objects.push_back(
                        std::make_unique<Registry>(
                            QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                            QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                        )
            );

            modelGroupModules.downloadRegistry();
            QCOMPARE(modelGroupModules.m_newVersionAvailable, false);
            QVERIFY(spy.wait());
            QCOMPARE(spyModulesUpdateCompleted.count(), 1);
            QCOMPARE(spy.count(), 1);
            QCOMPARE(modelGroupModules.m_updateCompleted, true);
            QCOMPARE(spyChangeUpdateCompleted.count(), 1);
            QCOMPARE(spyChangeNewVersionAvailable.count(), 1);
            QCOMPARE(modelGroupModules.m_objects.size(), static_cast<size_t>(2));
        }

        void tst_ModelGroupModules::updateObjects_data()
        {
            cleanTable();
            helperSave();
        }

        void tst_ModelGroupModules::updateObjects()
        {
            const auto &objects = helperGetObjectsUnique();

            ModelGroupModules modelGroupModules;
            modelGroupModules.updateObjects();
            QCOMPARE(modelGroupModules.m_objects.size(), objects.size());
            QCOMPARE(std::equal(dereference_iterator(modelGroupModules.m_objects.begin()),
                       dereference_iterator(modelGroupModules.m_objects.end()),
                       dereference_iterator(objects.begin())
                       ), true);
            QCOMPARE(modelGroupModules.objectsCount, 0);
        }

        void tst_ModelGroupModules::search_data()
        {
            cleanTable();
            helperSave();
            std::vector<GroupModulesShared>objects;
            objects.push_back(
                std::make_unique<GroupModules>("ru", "translate", "")
            );
            helperSave(std::move(objects));

            QTest::addColumn<QString>("needle");
            QTest::addColumn<std::vector<GroupModulesShared>>("objects");

            QTest::newRow("language exists English") << "eng" << helperGetObjects();
            QTest::newRow("not started language") << "env" << std::vector<GroupModulesShared>();
            QTest::newRow("language not exists Maithili") << "ma" << std::vector<GroupModulesShared>();
            QTest::newRow("started name") << "na" << helperGetObjects();
            QTest::newRow("not started name") << "me" << std::vector<GroupModulesShared>();
            QTest::newRow("started region") << "reg" << helperGetObjects();
        }

        void tst_ModelGroupModules::search()
        {
            qRegisterMetaType<std::vector<GroupModulesShared>>("std::vector<GroupModulesShared>");

            QFETCH(QString, needle);
            QFETCH(std::vector<GroupModulesShared>, objects);

            ModelGroupModules modelGroupModules;
            modelGroupModules.search(needle);
            QCOMPARE(modelGroupModules.m_objects.size(), objects.size());
            QCOMPARE(std::equal(dereference_iterator(modelGroupModules.m_objects.begin()),
                       dereference_iterator(modelGroupModules.m_objects.end()),
                       dereference_iterator(objects.begin())
                       ), true);
            QCOMPARE(modelGroupModules.objectsCount, 0);
        }

        void tst_ModelGroupModules::getAll_data()
        {
            updateObjects_data();
        }

        void tst_ModelGroupModules::getAll()
        {
            updateObjects();
        }

        void tst_ModelGroupModules::setFieldSearch_data()
        {
            QTest::addColumn<QString>("needle");
            QTest::addColumn<int>("entitySearch");
            QTest::addColumn<QString>("m_needle");

            QTest::newRow("needle: russ") << "russ" << 0 << "russ";
            QTest::newRow("needle: m russ") << "m russ" << 1 << "russ";
            QTest::newRow("needle: eng") << "eng" << 0 << "eng";
            QTest::newRow("needle: M eng") << "M eng" << 1 << "eng";
        }

        void tst_ModelGroupModules::setFieldSearch()
        {
            QFETCH(QString, needle);
            QFETCH(int, entitySearch);
            QFETCH(QString, m_needle);

            ModelGroupModules modelGroupModules;
            modelGroupModules.setFieldSearch(needle);

            QCOMPARE(modelGroupModules.m_entitySearch, entitySearch);
            QCOMPARE(modelGroupModules.m_needle, m_needle);
        }
    }
}

#include "tst_modelgroupmodules.moc"
