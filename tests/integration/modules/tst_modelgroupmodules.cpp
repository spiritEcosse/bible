#include <QtTest>
#include "modelgroupmodules.h"

Q_DECLARE_METATYPE(modules::Registry)

namespace modules {

    namespace tests {

        class tst_ModelGroupModules : public QObject {
            Q_OBJECT

        private:
            int vectorSize = 3;
            std::shared_ptr<db::Db<GroupModules>> m_db;

        public:
            tst_ModelGroupModules();
            ~tst_ModelGroupModules();

        private:
            std::vector<GroupModules> helperSave();
            std::vector<GroupModules> helperGetObjects();
            void cleanTable();

        private slots:
            void update();
        };

        //helpers

//        std::vector<Registry> tst_ModelGroupModules::helperGetRegistries() {
////            return std::vector<Registry> {{"bGluazE=", "bGluazEx", 1}, {"bGluazI=", "bGluazIy", 2}, {"bGluazM=", "bGluazMz", 3}};
//        }

//        std::vector<Registry> tst_ModelGroupModules::helperSaveRegistries()
//        {
////            const std::vector<Registry>& registries = helperGetRegistries();
////            m_db->storage->insert_range(registries.begin(), registries.end());
////            return registries;
//        }

        tst_ModelGroupModules::tst_ModelGroupModules()
            : m_db { db::Db<GroupModules>::getInstance() } {}

        tst_ModelGroupModules::~tst_ModelGroupModules() {}

        std::vector<GroupModules> tst_ModelGroupModules::helperGetObjects()
        {
            return std::vector<GroupModules> {{"en", "name", "region"}, {"en", "name", "region"}, {"en", "name", "region"}};
        }

        std::vector<GroupModules> tst_ModelGroupModules::helperSave()
        {
            const std::vector<GroupModules>& entries = helperGetObjects();
            m_db->storage->insert_range(entries.begin(), entries.end());
            return entries;
        }

        void tst_ModelGroupModules::cleanTable()
        {
            m_db->removeAll();
        }

        // tests
        void tst_ModelGroupModules::update()
        {
            ModelGroupModules modelGroupModules;
            QSignalSpy spyLast(&modelGroupModules, &ModelGroupModules::updateDone);

            modelGroupModules.update(helperGetObjects());

            QCOMPARE(spyLast.count(), 1);
            QCOMPARE(m_db->count(), vectorSize);
            QCOMPARE(modelGroupModules.m_objects.size(), helperGetObjects().size());
            QCOMPARE(modelGroupModules.m_objects, helperGetObjects());
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelGroupModules)

#include "tst_modelgroupmodules.moc"
