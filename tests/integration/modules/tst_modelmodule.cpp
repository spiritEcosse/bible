#include <QtTest>
#include "modelmodule.h"
#include "modeljsontest.h"
#include "dereferenceiterator.h"

namespace modules {

    namespace tests {

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
            void getExtraFields();
            void saveExtraFieldsToDb();
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
                                "abbreviation",
                                0,
                                102400,
                                "en",
                                "information",
                                "comment",
                                "copyright",
                                QDate(2017, 03, 31),
                                true,
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
                                true,
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

        void tst_ModelModule::getExtraFields()
        {
            cleanTable();
            helperSave();

            ModelModule model;
            model.getExtraFields();
        }

        void tst_ModelModule::saveExtraFieldsToDb()
        {
            cleanTable();
            helperSave();

            ModelModule model;
            model.saveExtraFieldsToDb();
        }
    }
}

QTEST_MAIN(modules::tests::tst_ModelModule)

#include "tst_modelmodule.moc"
