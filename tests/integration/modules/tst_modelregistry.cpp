#include <QtTest>
#include "modelregistry.h"

Q_DECLARE_METATYPE(modules::Registry)

namespace modules {

    namespace tests {

        class tst_ModelRegistry : public QObject {
            Q_OBJECT

        private:
            std::vector<Registry> helperGetObjects() const;
            std::vector<Registry> helperSaveRegistries() const;
            std::vector<Registry> helperGetBaseRegistries() const;
            void cleanTable();
            const size_t vectorSize = 3;
            std::shared_ptr<db::Db<Registry>> m_db;

        public:
            tst_ModelRegistry();
            ~tst_ModelRegistry();

        private slots:
            void setRegistries_data();
            void setRegistries();
            void update();
            void getRegistry_data();
            void getRegistry();
            void getRegistry_NoRegistry();
        };

        //helpers

        std::vector<Registry> tst_ModelRegistry::helperGetObjects() const {
            return std::vector<Registry> {vectorSize, {"bGluazE=", "bGluazEx", 1}};
        }

        std::vector<Registry> tst_ModelRegistry::helperSaveRegistries() const
        {
            const std::vector<Registry>& registries = helperGetObjects();
            m_db->save(registries.begin(), registries.end());
            return registries;
        }

        std::vector<Registry> tst_ModelRegistry::helperGetBaseRegistries() const
        {
            return std::vector<Registry> {{
                "aHR0cDovL21waDQucnUvcmVnaXN0cnkuemlw",
                "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u"
            }};
        }

        tst_ModelRegistry::tst_ModelRegistry()
            : m_db { db::Db<Registry>::getInstance() }
        {
        }

        tst_ModelRegistry::~tst_ModelRegistry()
        {

        }

        void tst_ModelRegistry::cleanTable()
        {
            m_db->removeAll();
        }

        // tests

        void tst_ModelRegistry::update()
        {
            ModelRegistry model;
            QSignalSpy spyLast(&model, &ModelRegistry::updateDone);

            const std::vector<Registry>& objects = helperGetObjects();
            model.update(objects);

            QCOMPARE(spyLast.count(), 1);
            QCOMPARE(m_db->count(), static_cast<int>(vectorSize));
            QCOMPARE(model.m_objects.size(), objects.size());
            QCOMPARE(model.m_objects, objects);
        }

        void tst_ModelRegistry::setRegistries_data()
        {
            cleanTable();

            QTest::addColumn<std::vector<Registry>>("m_registries");
            QTest::addColumn<bool>("result");

            QTest::newRow("exists rows in table") << helperSaveRegistries() << true;
            QTest::newRow("empty table") << helperGetBaseRegistries() << false;
        }

        void tst_ModelRegistry::setRegistries()
        {
            QFETCH(std::vector<Registry>, m_registries);
            QFETCH(bool, result);

            ModelRegistry modelRegistry;
            QCOMPARE(modelRegistry.setRegistries(), result);
            QCOMPARE(modelRegistry.m_objects, m_registries);

            cleanTable();
        }

        void tst_ModelRegistry::getRegistry_data()
        {
            cleanTable();

            QTest::addColumn<Registry>("registry");
            QTest::addColumn<int>("index");

            QTest::newRow("get registry without query to db") << helperGetBaseRegistries()[0] << 0;
            QTest::newRow("get registry with query (get a bunch of registers) to db") << helperSaveRegistries()[1] << 1;
        }

        void tst_ModelRegistry::getRegistry()
        {
            qRegisterMetaType<Registry>("Registry");

            QFETCH(Registry, registry);
            QFETCH(int, index);

            ModelRegistry modelRegistry;
            QSignalSpy spy(&modelRegistry, &ModelRegistry::registry);

            modelRegistry.index = index;
            modelRegistry.getRegistry();

            QList<QVariant> arguments = spy.takeFirst();
            QCOMPARE(arguments[0].value<Registry>(), registry);
        }

        void tst_ModelRegistry::getRegistry_NoRegistry()
        {
            cleanTable();

            ModelRegistry modelRegistry;
            QSignalSpy spy(&modelRegistry, &ModelRegistry::error);
            modelRegistry.index = 1;

            modelRegistry.getRegistry();
            QList<QVariant> arguments = spy.takeFirst();
            QCOMPARE(arguments[0].toString(), QString("An error occured, please try in time."));
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelRegistry)

#include "tst_modelregistry.moc"
