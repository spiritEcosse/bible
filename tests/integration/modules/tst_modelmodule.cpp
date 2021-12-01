#include "tst_modelgroupmodules.h"
#include "dereferenceiterator.h"
#include "quickdownload.h"
#include <chrono>
#include "tst_modelmodule.h"

namespace modules {

    namespace tests {

        using namespace sqlite_orm;

        tst_ModelModule::tst_ModelModule() {}

        tst_ModelModule::~tst_ModelModule() {}

        void tst_ModelModule::initTestCase()
        {
            ModelJsonTest<Module, ModelModule>::initTestCase();
        }

        void tst_ModelModule::cleanupTestCase()
        {
            ModelJsonTest<Module, ModelModule>::cleanupTestCase();
        }

        //helpers
        std::vector<ModuleShared> tst_ModelModule::helperGetObjects() const
        {
            std::vector<ModuleShared> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(
                            std::make_shared<Module>(
                                QString("name.%1").arg(in),
                                "description",
                                QString("abbreviation.%1").arg(in),
                                in + 1,
                                102400,
                                "en",
                                "information",
                                "comment",
                                "copyright",
                                QDate(2017, 03, 31),
                                false,
                                false,
                                true)
                );
            }
            return objects;
        }

        std::vector<ModuleUnique> tst_ModelModule::helperGetObjectsUnique() const
        {
            std::vector<ModuleUnique> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(
                            std::make_unique<Module>(
                                QString("name.%1").arg(in),
                                "description",
                                QString("abbreviation.%1").arg(in),
                                in + 1,
                                102400,
                                "en",
                                "information",
                                "comment",
                                "copyright",
                                QDate(2017, 03, 31),
                                false,
                                false,
                                true)
                );
            }
            return objects;
        }

        HostUnique tst_ModelModule::helperGetHostUnique() const
        {
            return std::make_unique<modules::Host>("alias", urlModuleToBase64(), 1, 2);
        }

        QVariantList tst_ModelModule::helperGetSelected() const
        {
            QMap<QString, QVariant> objects;
            objects.insert("moduleId", 1);
            return QVariantList({objects});
        }

        HostUnique tst_ModelModule::helperGetHostsUniqueNotExists() const
        {
            return std::make_unique<modules::Host>("alias", QString(strUrl + urlMask).toUtf8().toBase64(), 1, 2);
        }

        std::vector<modules::HostUnique> tst_ModelModule::helperGetHostsUnique()
        {
            std::vector<modules::HostUnique> objects;
            objects.push_back(helperGetHostUnique());
            return objects;
        }

        // tests
        void tst_ModelModule::update()
        {
            ModelJsonTest<Module, ModelModule>::update();
        }

        void tst_ModelModule::init_model()
        {
            ModelModule model;
            QSignalSpy spySelected(&model, &ModelModule::changeSelected);
            QSignalSpy spyDownloaded(&model, &ModelModule::changeDownloaded);
            model.init();
            model.init();
            QCOMPARE(spySelected.count(), 1);
            QCOMPARE(spyDownloaded.count(), 1);
        }

        void tst_ModelModule::updateObjects_data()
        {
            cleanTable();
            helperSave();
        }

        void tst_ModelModule::updateObjects()
        {
            const auto &objects = helperGetObjectsUnique();

            ModelModule modelModule;
            modelModule.updateObjects();
            QCOMPARE(modelModule.m_objects.size(), objects.size());
            QCOMPARE(std::equal(dereference_iterator(modelModule.m_objects.begin()),
                       dereference_iterator(modelModule.m_objects.end()),
                       dereference_iterator(objects.begin())
                       ), true);
            QCOMPARE(modelModule.objectsCount, 0);
        }

        void tst_ModelModule::updateObjectsDownloaded()
        {
            cleanTable();
            helperSave();

            tst_ModelGroupModules::helperSaveStatic();

            const auto &objects = helperGetObjectsUnique();

            ModelModule modelModule;
            modelModule.updateObjectsDownloaded();
            QCOMPARE(modelModule.m_objects.size(), objects.size());
            QCOMPARE(std::equal(dereference_iterator(modelModule.m_objects.begin()),
                       dereference_iterator(modelModule.m_objects.end()),
                       dereference_iterator(objects.begin())
                       ), true);
            QCOMPARE(modelModule.objectsCount, 0);
        }

        void tst_ModelModule::updateSelected_data()
        {
            QTest::addColumn<bool>("value");

            QTest::newRow("m_selecting is true") << true;
            QTest::newRow("m_selecting is false") << false;
        }

        void tst_ModelModule::updateSelected()
        {
            QFETCH(bool, value);

            cleanTable();
            helperSave();

            ModelModule model;
            int id = 1;
            model.updateSelected(id, value);
            const auto &object = m_db->storage->get_pointer<Module>(id);
            QCOMPARE(object->m_selected, value);
        }

        void tst_ModelModule::updateDownloaded_data()
        {
            QTest::addColumn<bool>("value");

            QTest::newRow("m_downloaded is true") << true;
            QTest::newRow("m_downloaded is false") << false;
        }

        void tst_ModelModule::updateDownloaded()
        {
            QFETCH(bool, value);

            cleanTable();
            helperSave();

            ModelModule model;
            int id = 1;
            model.updateDownloaded(id, value);
            const auto &object = m_db->storage->get_pointer<Module>(id);
            QCOMPARE(object->m_downloaded, value);
        }

        void tst_ModelModule::getExtraFieldsFromDb()
        {
            cleanTable();
            helperSave();

            m_db->storage->update_all(set(assign(&Module::m_downloaded, true)));
            m_db->storage->update_all(set(assign(&Module::m_selected, true)));

            ModelModule model;
            model.getExtraFieldsFromDb();

            QCOMPARE(model.m_selectedBackup.size(), vectorSize);
            QCOMPARE(model.m_downloadedBackup.size(), vectorSize);
        }

        void tst_ModelModule::saveExtraFieldsToDb_data()
        {
            cleanTable();
            helperSave();

            ModelModule model;
            model.m_downloadedBackup.push_back(std::make_tuple("name.0"));
            model.m_downloadedBackup.push_back(std::make_tuple("name.2"));

            model.m_selectedBackup.push_back(std::make_tuple("name.0"));
            model.m_selectedBackup.push_back(std::make_tuple("name.2"));

            model.saveExtraFieldsToDb();

            QTest::addColumn<int>("id");
            QTest::addColumn<bool>("selected");
            QTest::addColumn<bool>("downloaded");

            QTest::newRow("name.0 m_downloaded and m_selected is true") << 1 << true << true;
            QTest::newRow("name.1 m_downloaded and m_selected is false") << 2 << false << false;
            QTest::newRow("name.2 m_downloaded and m_selected is true") << 3 << true << true;
            QCOMPARE(int(model.m_selectedBackup.size()), 0);
            QCOMPARE(int(model.m_downloadedBackup.size()), 0);
        }

        void tst_ModelModule::saveExtraFieldsToDb()
        {
            QFETCH(int, id);
            QFETCH(bool, selected);
            QFETCH(bool, downloaded);

            const auto &object = m_db->storage->get_pointer<Module>(id);
            QCOMPARE(object->m_downloaded, downloaded);
            QCOMPARE(object->m_selected, selected);
        }

        void tst_ModelModule::downloadModules_data()
        {
            cleanTable();
            helperSave();
            createFileModule();
            std::unique_ptr<db::Db<modules::Host>> m_dbHost;
            m_dbHost.reset(new db::Db<modules::Host>());
            m_dbHost->removeAll();

            auto entries = helperGetHostsUnique();
            m_dbHost->save(entries.begin(), entries.end());

            QTest::addColumn<int>("spyDownloadedCount");
            QTest::newRow("updateSuccessfullyDownloaded") << 1;
            QTest::newRow("retryFailedDownloaded") << 2;
        }

        void tst_ModelModule::downloadModules()
        {
            QFETCH(int, spyDownloadedCount);

            ModelModule model;
            if (spyDownloadedCount == 2) {
                model.m_worker->m_modelHost->m_objects.clear();
                model.m_worker->m_modelHost->m_objects.push_back(helperGetHostsUniqueNotExists());
            } else {
                model.m_worker->m_modelHost->m_objects = helperGetHostsUnique();
            }
            QCOMPARE(m_db->storage->get<Module>(1).m_downloaded, false);
            model.downloadModules(helperGetSelected());
            QSignalSpy spy(&model, &ModelModule::changeDownloaded);
            QSignalSpy spyChangeDownloadCompleted(&model, &ModelModule::changeDownloadCompleted);
            QVERIFY(spy.wait());
            QCOMPARE(spy.count(), 1);
            QCOMPARE(spyChangeDownloadCompleted.count(), 1);
            QCOMPARE(m_db->storage->get<Module>(1).m_downloaded, true);
            QVERIFY(folderModuleInModules.exists());
            QVERIFY(!QFile::exists(getModuleFilePath(fileModuleArchiveInModules)));

            cleanTable();
            helperSave();
        }

        void tst_ModelModule::downloadModules_withoutRecursion()
        {
            std::unique_ptr<db::Db<modules::Host>> m_dbHost;
            m_dbHost.reset(new db::Db<modules::Host>());
            m_dbHost->removeAll();

//            auto start = std::chrono::system_clock::now();
            ModelModule model;
            SingletonModelHost::getInstance().m_objects.clear();
            model.m_worker->m_modelHost->m_objects.clear();
            model.m_worker->m_modelHost->m_objects.push_back(helperGetHostsUniqueNotExists());
            QSignalSpy spy(&model, &ModelModule::changeDownloaded);
            QSignalSpy spyChangeDownloadCompleted(&model, &ModelModule::changeDownloadCompleted);
            QCOMPARE(model.m_downloadCompleted, true);
            model.downloadModules(helperGetSelected());
            QCOMPARE(model.m_downloadCompleted, false);
            QVERIFY(spy.wait());
            QCOMPARE(spy.count(), 1);
            QCOMPARE(spyChangeDownloadCompleted.count(), 2);
            QCOMPARE(model.m_downloadCompleted, true);
//            auto end = std::chrono::system_clock::now();
//            std::chrono::duration<double> elapsed_seconds = end-start;
//            std::time_t end_time = std::chrono::system_clock::to_time_t(end);

//            qDebug() << "elapsed time: " << elapsed_seconds.count();
//            QCOMPARE(spyCurlDownloaded.count(), 1);
        }

        void tst_ModelModule::deleteModules()
        {
            cleanTable();
            helperSave();
            createFolderModuleInModules();

            m_db->storage->update_all(set(assign(&Module::m_downloaded, true)));
            QCOMPARE(m_db->storage->get<Module>(1).m_downloaded, true);

            ModelModule model;
            QSignalSpy spy(&model, &ModelModule::changeDownloaded);
            QSignalSpy spyChangeDeleteCompleted(&model, &ModelModule::changeDeleteCompleted);
            QCOMPARE(model.m_deleteCompleted, true);
            model.deleteModules(helperGetSelected());
            QCOMPARE(model.m_deleteCompleted, false);
            QVERIFY(spy.wait());
            QCOMPARE(spy.count(), 1);
            QCOMPARE(spyChangeDeleteCompleted.count(), 2);
            QCOMPARE(m_db->storage->get<Module>(1).m_downloaded, false);
            QCOMPARE(model.m_deleteCompleted, true);
            QVERIFY(!folderModuleInModules.exists());
        }

        void tst_ModelModule::retrieveDownloaded()
        {
            cleanTable();
            helperSave();
            m_db->storage->update_all(set(assign(&Module::m_downloaded, true)));

            ModelModule model;
            model.retrieveDownloaded();
        }

        void tst_ModelModule::retrieveSelected()
        {
            cleanTable();
            helperSave();
            m_db->storage->update_all(set(assign(&Module::m_selected, true)));

            ModelModule model;
            model.retrieveSelecteded();
        }
    }
}

#include "tst_modelmodule.moc"
