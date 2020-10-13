#include <QtTest>
#include "modelregistry.h"

Q_DECLARE_METATYPE(modules::Registry)

namespace modules {

    namespace tests {

        class tst_ModelRegistry : public QObject {
            Q_OBJECT

        private:
            QJsonDocument helperGetDocument();
            std::vector<Registry> helpGetRegistries();
            std::vector<Registry> helpSaveRegistries();
            int vectorSize = 3;
            std::shared_ptr<db::Db> m_db;

        public:
            tst_ModelRegistry();
            ~tst_ModelRegistry();

        private slots:
            void cleanRegistryTable();
            void update();
            void deleteAllRegistries();
            void saveRegistries();
            void getRegistry_data();
            void getRegistry();
            void getRegistry_NoRegistry();
        };

        //helpers

        QJsonDocument tst_ModelRegistry::helperGetDocument()
        {
            QJsonArray array;

            array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};
            array << QJsonObject {{"url", "link2"}, {"priority", 2}, {"info_url", "link22"}};
            array << QJsonObject {{"url", "link3"}, {"priority", 3}, {"info_url", "link33"}};

            return QJsonDocument {
                QJsonObject {
                    { "registries",  array }
                }
            };
        }

        std::vector<Registry> tst_ModelRegistry::helpGetRegistries() {
            return std::vector<Registry> {{"bGluazE=", 1, "bGluazEx"}, {"bGluazI=", 2, "bGluazIy"}, {"bGluazM=", 3, "bGluazMz"}};
        }

        std::vector<Registry> tst_ModelRegistry::helpSaveRegistries()
        {
            const std::vector<Registry>& registries = helpGetRegistries();
            m_db->storage->insert_range(registries.begin(), registries.end());
            return registries;
        }

        tst_ModelRegistry::tst_ModelRegistry()
            : m_db { db::Db::getInstance() }
        {
        }

        tst_ModelRegistry::~tst_ModelRegistry()
        {

        }

        void tst_ModelRegistry::cleanRegistryTable()
        {
            m_db->storage->remove_all<Registry>();
        }

        // tests
        void tst_ModelRegistry::update()
        {
            cleanRegistryTable();

            ModelRegistry modelRegistry;
            QSignalSpy spyLast(&modelRegistry, &ModelRegistry::updateSuccess);

            helpSaveRegistries();

            modelRegistry.update(helperGetDocument());

            QCOMPARE(spyLast.count(), 1);
            QCOMPARE(m_db->storage->count<Registry>(), vectorSize);
            QCOMPARE(modelRegistry.m_registries, helpGetRegistries());
        }

        void tst_ModelRegistry::deleteAllRegistries()
        {
            ModelRegistry modelRegistry;

            helpSaveRegistries();

            modelRegistry.deleteAllRegistries();
            QCOMPARE(m_db->storage->count<Registry>(), 0);
        }

        void tst_ModelRegistry::saveRegistries()
        {
            cleanRegistryTable();

            ModelRegistry modelRegistry;

            modelRegistry.saveRegistries(helpGetRegistries());
            QCOMPARE(m_db->storage->count<Registry>(), vectorSize);
        }

        void tst_ModelRegistry::getRegistry_data()
        {
            cleanRegistryTable();

            QTest::addColumn<Registry>("registry");
            QTest::addColumn<int>("index");

            Registry registry {
                "aHR0cDovL21waDQucnUvcmVnaXN0cnkuemlw",
                1,
                "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u"
            };

            QTest::newRow("get registry without query to db") << registry << 0;
            QTest::newRow("get registry with query (get a bunch of registers) to db") << helpSaveRegistries()[1] << 1;
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
            cleanRegistryTable();

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
