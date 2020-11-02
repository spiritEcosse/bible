#include <QtTest>
#include "modelmodule.h"

namespace modules {

    namespace tests {

        class tst_ModelModule : public QObject {
            Q_OBJECT

        private:
            const size_t vectorSize = 3;
            std::shared_ptr<db::Db<Module>> m_db;

        public:
            tst_ModelModule();
            ~tst_ModelModule();

        private:
            std::vector<Module> helperSave() const;
            std::vector<Module> helperGetObjects() const;
            void cleanTable();

        private slots:
            void update();
        };


        tst_ModelModule::tst_ModelModule()
            : m_db { db::Db<Module>::getInstance() } {}

        tst_ModelModule::~tst_ModelModule() {}

        //helpers
        std::vector<Module> tst_ModelModule::helperGetObjects() const
        {
            return std::vector<Module> {vectorSize, {"name", "des", "abb"}};
        }

        std::vector<Module> tst_ModelModule::helperSave() const
        {
            const std::vector<Module>& entries = helperGetObjects();
            m_db->save(entries.begin(), entries.end());
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

            const std::vector<Module>& objects = helperGetObjects();
            model.update(objects);

            QCOMPARE(spyLast.count(), 1);
            QCOMPARE(m_db->count(), static_cast<int>(vectorSize));
            QCOMPARE(model.m_objects.size(), objects.size());
            QCOMPARE(model.m_objects, objects);
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelModule)

#include "tst_modelmodule.moc"
