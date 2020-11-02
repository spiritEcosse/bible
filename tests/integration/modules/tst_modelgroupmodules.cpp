#include <QtTest>
#include "modelgroupmodules.h"

namespace modules {

    namespace tests {

        class tst_ModelGroupModules : public QObject {
            Q_OBJECT

        private:
            const size_t vectorSize = 3;
            std::shared_ptr<db::Db<GroupModules>> m_db;

        public:
            tst_ModelGroupModules();
            ~tst_ModelGroupModules();

        private:
            std::vector<GroupModules> helperSave() const;
            std::vector<GroupModules> helperGetObjects() const;
            void cleanTable();

        private slots:
            void update();
        };


        tst_ModelGroupModules::tst_ModelGroupModules()
            : m_db { db::Db<GroupModules>::getInstance() } {}

        tst_ModelGroupModules::~tst_ModelGroupModules() {}

        //helpers
        std::vector<GroupModules> tst_ModelGroupModules::helperGetObjects() const
        {
            return std::vector<GroupModules> {vectorSize, {"en", "name", "region"}};
        }

        std::vector<GroupModules> tst_ModelGroupModules::helperSave() const
        {
            const std::vector<GroupModules>& entries = helperGetObjects();
            m_db->save(entries.begin(), entries.end());
            return entries;
        }

        void tst_ModelGroupModules::cleanTable()
        {
            m_db->removeAll();
        }

        // tests
        void tst_ModelGroupModules::update()
        {
            ModelGroupModules model;
            QSignalSpy spyLast(&model, &ModelGroupModules::updateDone);

            const std::vector<GroupModules> objects = helperGetObjects();
            model.update(objects);

            QCOMPARE(spyLast.count(), 1);
            QCOMPARE(m_db->count(), static_cast<int>(vectorSize));
            QCOMPARE(model.m_objects.size(), objects.size());
            QCOMPARE(model.m_objects, objects);
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelGroupModules)

#include "tst_modelgroupmodules.moc"
