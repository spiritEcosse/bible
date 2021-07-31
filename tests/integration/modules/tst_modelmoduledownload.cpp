#include <QtTest>
#include "modelmoduledownload.h"
#include "basetest.h"
#include "dereferenceiterator.h"
#include "QObject"

namespace modules {

    namespace tests {

        class tst_ModelModuleDownload : public ::tests::BaseTest<ModuleDownload, ModelModuleDownload> {
            Q_OBJECT

        public:
            tst_ModelModuleDownload();
            ~tst_ModelModuleDownload();

        private:
            std::vector<ModuleDownloadShared> helperGetObjects() const override;
            std::vector<ModuleDownloadUnique> helperGetObjectsUnique() const override;
            std::unique_ptr<db::Db<Module>> m_dbModule;
        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void updateObjects_data();
            void updateObjects();
            void crudWithSelecting();
            void crudWithDownloaded();
            void remove();
        };

        tst_ModelModuleDownload::tst_ModelModuleDownload() {}

        tst_ModelModuleDownload::~tst_ModelModuleDownload() {}

        void tst_ModelModuleDownload::initTestCase()
        {
            ::tests::BaseTest<ModuleDownload, ModelModuleDownload>::initTestCase();
            m_dbModule.reset(new db::Db<Module>());
        }

        void tst_ModelModuleDownload::cleanupTestCase()
        {
            ::tests::BaseTest<ModuleDownload, ModelModuleDownload>::cleanupTestCase();
        }

        //helpers
        std::vector<ModuleDownloadShared> tst_ModelModuleDownload::helperGetObjects() const
        {
            std::vector<ModuleDownloadShared> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(
                            std::make_shared<ModuleDownload>(
                                QString("abbreviation%1").arg(in+1),
                                false,
                                false,
                                in + 1,
                                in + 1,
                                in + 1)
                );
            }
            return objects;
        }

        std::vector<ModuleDownloadUnique> tst_ModelModuleDownload::helperGetObjectsUnique() const
        {
            std::vector<ModuleDownloadUnique> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(
                            std::make_unique<ModuleDownload>(
                                QString("abbreviation%1").arg(in+1),
                                false,
                                false,
                                in + 1,
                                in + 1,
                                in + 1)
                );
            }
            return objects;
        }

        // tests
        void tst_ModelModuleDownload::updateObjects_data()
        {
            cleanTable();
            helperSave();
            m_dbModule->removeAll();
            std::vector<ModuleUnique> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(
                            std::make_unique<Module>(
                                "name",
                                "description",
                                QString("abbreviation%1").arg(in+1),
                                in + 1,
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
            m_dbModule->save(objects.begin(), objects.end());
        }

        void tst_ModelModuleDownload::updateObjects()
        {
            const auto &objects = helperGetObjectsUnique();

            ModelModuleDownload model;
            model.updateObjects();
            QCOMPARE(model.m_objects.size(), objects.size());
            QCOMPARE(std::equal(dereference_iterator(model.m_objects.begin()),
                       dereference_iterator(model.m_objects.end()),
                       dereference_iterator(objects.begin())
                       ), true);
            QCOMPARE(model.objectsCount, static_cast<int>(objects.size()));
        }

        void tst_ModelModuleDownload::crudWithSelecting()
        {
            cleanTable();
            helperSave();

            ModelModuleDownload model;
            //create
            model.crudWithSelecting("abbreviation4", true);
            const auto &object = m_db->storage->get_pointer<ModuleDownload>(4);
            QVERIFY(object->m_selecting);
            QCOMPARE(object->m_downloaded, false);
            QCOMPARE(m_db->count(), 4);

            // update
            model.crudWithSelecting("abbreviation1", true);
            const auto &object1 = m_db->storage->get_pointer<ModuleDownload>(1);
            QVERIFY(object1->m_selecting);
            QCOMPARE(object1->m_downloaded, false);
            QCOMPARE(m_db->count(), 4);

            //delete
            model.crudWithSelecting("abbreviation1", false);
            QCOMPARE(m_db->count(), 3);
        }

        void tst_ModelModuleDownload::crudWithDownloaded()
        {
            cleanTable();
            helperSave();

            ModelModuleDownload model;
            //create
            model.crudWithDownloaded("abbreviation4", true);
            const auto &object = m_db->storage->get_pointer<ModuleDownload>(4);
            QVERIFY(object->m_downloaded);
            QCOMPARE(object->m_selecting, false);
            QCOMPARE(m_db->count(), 4);

            // update
            model.crudWithDownloaded("abbreviation1", true);
            const auto &object1 = m_db->storage->get_pointer<ModuleDownload>(1);
            QVERIFY(object1->m_downloaded);
            QCOMPARE(object1->m_selecting, false);
            QCOMPARE(m_db->count(), 4);

            //delete
            model.crudWithDownloaded("abbreviation1", false);
            QCOMPARE(m_db->count(), 3);
        }

        void tst_ModelModuleDownload::remove()
        {
            cleanTable();
            helperSave();

            auto objects = helperGetObjectsUnique();

            ModelModuleDownload model;
            model.remove(1);
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelModuleDownload)

#include "tst_modelmoduledownload.moc"
