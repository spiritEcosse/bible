#include <QtTest>
#include "modelregistry.h"

Q_DECLARE_METATYPE(modules::Registry)

namespace modules {

    namespace tests {

        class tst_ModelRegistry : public QObject {
            Q_OBJECT

        private:
            QJsonDocument helperGetDocument();
            QJsonDocument helperGetInvalidDocument();
            std::vector<Registry> helperGetRegistries();
            std::vector<Registry> helperSaveRegistries();
            std::vector<Registry> helperGetBaseRegistries();
            int vectorSize = 3;
            std::shared_ptr<db::Db> m_db;

        public:
            tst_ModelRegistry();
            ~tst_ModelRegistry();

        private slots:
            void cleanRegistryTable();
            void setRegistries_data();
            void setRegistries();
            void update();
            void update_data();
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

        QJsonDocument tst_ModelRegistry::helperGetInvalidDocument()
        {
            QJsonArray array;

            array << QJsonObject {{"url", "link1"}, {"priority", 1}};
            array << QJsonObject {{"priority", 2}, {"info_ufrl", "link22"}};
            array << QJsonObject {{"url", "link3"}, {"priority", 3}, {"info_url", "link33"}};

            return QJsonDocument {
                QJsonObject {
                    { "registries",  array }
                }
            };
        }

        std::vector<Registry> tst_ModelRegistry::helperGetRegistries() {
            return std::vector<Registry> {{"bGluazE=", "bGluazEx", 1}, {"bGluazI=", "bGluazIy", 2}, {"bGluazM=", "bGluazMz", 3}};
        }

        std::vector<Registry> tst_ModelRegistry::helperSaveRegistries()
        {
            const std::vector<Registry>& registries = helperGetRegistries();
            m_db->storage->insert_range(registries.begin(), registries.end());
            return registries;
        }

        std::vector<Registry> tst_ModelRegistry::helperGetBaseRegistries()
        {
            return std::vector<Registry> {{
                "aHR0cDovL21waDQucnUvcmVnaXN0cnkuemlw",
                "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u"
            }};
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
        void tst_ModelRegistry::update_data()
        {
            QTest::addColumn<QJsonDocument>("document");
            QTest::addColumn<std::vector<Registry>>("m_registries");
            QTest::addColumn<int>("size");

            QTest::newRow("success") << helperGetDocument() << helperGetRegistries() << vectorSize ;
            QTest::newRow("invalid data") << helperGetInvalidDocument() << helperGetBaseRegistries() << 0;
        }

        void tst_ModelRegistry::update()
        {
            cleanRegistryTable();

            QFETCH(QJsonDocument, document);
            QFETCH(std::vector<Registry>, m_registries);
            QFETCH(int, size);

            ModelRegistry modelRegistry;
            QSignalSpy spyLast(&modelRegistry, &ModelRegistry::updateDone);

            modelRegistry.update(document);

            QCOMPARE(spyLast.count(), 1);
            QCOMPARE(m_db->storage->count<Registry>(), size);
            QCOMPARE(modelRegistry.m_registries, m_registries);
        }

        void tst_ModelRegistry::deleteAllRegistries()
        {
            ModelRegistry modelRegistry;

            helperSaveRegistries();

            modelRegistry.deleteAllRegistries();
            QCOMPARE(m_db->storage->count<Registry>(), 0);
        }

        void tst_ModelRegistry::setRegistries_data()
        {
            cleanRegistryTable();

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
            QCOMPARE(modelRegistry.m_registries, m_registries);

            cleanRegistryTable();
        }

        void tst_ModelRegistry::saveRegistries()
        {
            cleanRegistryTable();

            ModelRegistry modelRegistry;

            modelRegistry.saveRegistries(helperGetRegistries());
            QCOMPARE(m_db->storage->count<Registry>(), vectorSize);
        }

        void tst_ModelRegistry::getRegistry_data()
        {
            cleanRegistryTable();

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
