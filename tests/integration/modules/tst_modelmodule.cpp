#include <QtTest>
#include "modelmodule.h"
#include "modeljsontest.h"
#include "dereferenceiterator.h"
#include "quickdownload.h"


namespace modules {

    namespace tests {

        using namespace sqlite_orm;

        class tst_ModelModule : public ::tests::ModelJsonTest<Module, ModelModule> {
            Q_OBJECT

        public:
            tst_ModelModule();
            ~tst_ModelModule();

        private:
            std::vector<ModuleShared> helperGetObjects() const override;
            std::vector<ModuleUnique> helperGetObjectsUnique() const override;
        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void update() override;
            void updateObjects_data();
            void updateObjects();
            void updateSelecting_data();
            void updateSelecting();
            void updateDownloaded_data();
            void updateDownloaded();
//            void getExtraFields();
            void getExtraFieldsFromDb();
            void saveExtraFieldsToDb();
            void saveExtraFieldsToDb_data();
            void downloadModules();
        };

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
                                "name",
                                "description",
                                QString("abbreviation_%1").arg(in),
                                0,
                                102400,
                                "en",
                                "information",
                                "comment",
                                "copyright",
                                QDate(2017, 03, 31),
                                false,
                                false)
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
                                "name",
                                "description",
                                "abbreviation",
                                0,
                                102400,
                                "en",
                                "information",
                                "comment",
                                "copyright",
                                QDate(2017, 03, 31),
                                false,
                                false)
                );
            }
            return objects;
        }

        // tests
        void tst_ModelModule::update()
        {
//            ModelJsonTest<Module, ModelModule>::update();
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
//            QCOMPARE(std::equal(dereference_iterator(modelModule.m_objects.begin()),
//                       dereference_iterator(modelModule.m_objects.end()),
//                       dereference_iterator(objects.begin())
//                       ), true);
            QCOMPARE(modelModule.objectsCount, 0);
        }

        void tst_ModelModule::updateSelecting_data()
        {
            QTest::addColumn<bool>("value");

            QTest::newRow("m_selecting is true") << true;
            QTest::newRow("m_selecting is false") << false;
        }

        void tst_ModelModule::updateSelecting()
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

            QCOMPARE(model.selected->size(), vectorSize);
            QCOMPARE(model.downloaded->size(), vectorSize);
        }

        void tst_ModelModule::saveExtraFieldsToDb_data()
        {
            cleanTable();
            helperSave();

            ModelModule model;
            model.downloaded = std::make_unique<ModelModule::Downloaded> ();
            model.downloaded->push_back(std::make_tuple("abbreviation_0"));
            model.downloaded->push_back(std::make_tuple("abbreviation_2"));

            model.selected = std::make_unique<ModelModule::Downloaded> ();
            model.selected->push_back(std::make_tuple("abbreviation_0"));
            model.selected->push_back(std::make_tuple("abbreviation_2"));
            model.saveExtraFieldsToDb();

            QTest::addColumn<int>("id");
            QTest::addColumn<bool>("selected");
            QTest::addColumn<bool>("downloaded");

            QTest::newRow("abbreviation_0 m_downloaded and m_selected is true") << 1 << true << true;
            QTest::newRow("abbreviation_1 m_downloaded and m_selected is false") << 2 << false << false;
            QTest::newRow("abbreviation_2 m_downloaded and m_selected is true") << 3 << true << true;
            QCOMPARE(int(model.selected->size()), 0);
            QCOMPARE(int(model.downloaded->size()), 0);
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

        void tst_ModelModule::downloadModules()
        {
            std::unique_ptr<ModelModule::Downloaded> downloaded = std::make_unique<ModelModule::Downloaded> ();
            downloaded->push_back(std::make_tuple("AGP"));
            downloaded->push_back(std::make_tuple("ARC"));

            ModelModule model;
            model.downloadModules({QUrl("https://sabnzbd.org/tests/internetspeed/50MB.bin")});
        }
    }
}

QTEST_MAIN(modules::tests::tst_ModelModule)

#include "tst_modelmodule.moc"
