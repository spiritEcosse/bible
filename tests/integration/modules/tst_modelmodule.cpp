#include <QtTest>
#include "modelmodule.h"

namespace modules {

    namespace tests {

        class tst_ModelModule : public QObject {
            Q_OBJECT

        private:
            int vectorSize = 3;
            std::shared_ptr<db::Db<Module>> m_db;

        public:
            tst_ModelModule();
            ~tst_ModelModule();

        private:
            std::vector<Module> helperSave();
            std::vector<Module> helperGetObjects();
            void cleanTable();

        private slots:
            void update();
        };

        //helpers

        tst_ModelModule::tst_ModelModule()
            : m_db { db::Db<Module>::getInstance() } {}

        tst_ModelModule::~tst_ModelModule() {}

        std::vector<Module> tst_ModelModule::helperGetObjects()
        {
            return std::vector<Module> {{"name", "des", "abb"}, {"name", "des", "abb"}, {"name", "des", "abb"}};
        }

        std::vector<Module> tst_ModelModule::helperSave()
        {
            const std::vector<Module>& entries = helperGetObjects();
            m_db->storage->insert_range(entries.begin(), entries.end());
            return entries;
        }

        void tst_ModelModule::cleanTable()
        {
            m_db->removeAll();
        }

        // tests
        void tst_ModelModule::update()
        {
            ModelModule model;
            QSignalSpy spyLast(&model, &ModelModule::updateDone);

            const std::vector<Module> objects = helperGetObjects();
            model.update(objects);

            QCOMPARE(spyLast.count(), 1);
            QCOMPARE(m_db->count(), vectorSize);
            QCOMPARE(model.m_objects.size(), objects.size());
            QCOMPARE(model.m_objects, objects);
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelModule)

#include "tst_modelmodule.moc"
