#include <QtTest>
#include "modelregistry.h"
#include "basetest.h"

Q_DECLARE_METATYPE(modules::Registry)

namespace modules {

    namespace tests {

        class tst_ModelRegistry : public ::tests::BaseTest<Registry, ModelRegistry> {
            Q_OBJECT

        private:
            std::vector<Registry> helperGetObjects() const override;
            std::vector<Registry> helperGetBaseRegistries() const;

        public:
            tst_ModelRegistry();
            ~tst_ModelRegistry();

        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void update() override;
            void setRegistries_data();
            void setRegistries();
            void getRegistry_data();
            void getRegistry();
            void getRegistry_NoRegistry();
        };

        tst_ModelRegistry::tst_ModelRegistry() {}

        tst_ModelRegistry::~tst_ModelRegistry() {}

        void tst_ModelRegistry::initTestCase()
        {
            ::tests::BaseTest<Registry, ModelRegistry>::initTestCase();
        }

        void tst_ModelRegistry::cleanupTestCase()
        {
            ::tests::BaseTest<Registry, ModelRegistry>::cleanupTestCase();
        }

        //helpers

        std::vector<Registry> tst_ModelRegistry::helperGetObjects() const {
            return std::vector<Registry> {vectorSize, {"bGluazE=", "bGluazEx", 1}};
        }

        std::vector<Registry> tst_ModelRegistry::helperGetBaseRegistries() const
        {
            return std::vector<Registry> {{
                "aHR0cDovL21waDQucnUvcmVnaXN0cnkuemlw",
                "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u"
            }};
        }

        // tests

        void tst_ModelRegistry::update()
        {
            ::tests::BaseTest<Registry, ModelRegistry>::update();
        }

        void tst_ModelRegistry::setRegistries_data()
        {
            cleanTable();

            QTest::addColumn<std::vector<Registry>>("m_registries");
            QTest::addColumn<bool>("result");

            QTest::newRow("exists rows in table") << helperSave() << true;
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
            QTest::newRow("get registry with query (get a bunch of registers) to db") << helperSave()[1] << 1;
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
