#include <QtTest>
#include "modelregistry.h"
#include "basetest.h"
#include "dereferenceiterator.h"

Q_DECLARE_METATYPE(modules::RegistryShared)
Q_DECLARE_METATYPE(std::vector<modules::RegistryShared>)

namespace modules {

    namespace tests {

        class tst_ModelRegistry : public ::tests::BaseTest<Registry, ModelRegistry>  {
            Q_OBJECT

        private:
            std::vector<RegistryShared> helperGetObjects() const override;
            std::vector<RegistryShared> helperGetBaseRegistries() const;
            std::vector<RegistryUnique> helperGetObjectsUnique() const override;

        public:
            tst_ModelRegistry();
            ~tst_ModelRegistry();

        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void update() override;
            void setRegistries_data();
            void setRegistries();
            void constructor();
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

        std::vector<RegistryShared> tst_ModelRegistry::helperGetObjects() const
        {
            std::vector<RegistryShared> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_shared<Registry>("bGluazE=", "bGluazEx", 1));
            }
            return objects;
        }

        std::vector<RegistryShared> tst_ModelRegistry::helperGetBaseRegistries() const
        {
            std::vector<RegistryShared> objects;
            objects.push_back(
                        std::make_shared<Registry>(
                            "aHR0cDovL21waDQucnUvcmVnaXN0cnkuemlw",
                            "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u"
                        )
            );
            return objects;
        }

        std::vector<RegistryUnique> tst_ModelRegistry::helperGetObjectsUnique() const
        {
            std::vector<RegistryUnique> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_unique<Registry>("bGluazE=", "bGluazEx", 1));
            }
            return objects;
        }

        // tests

        void tst_ModelRegistry::update()
        {
            ::tests::BaseTest<Registry, ModelRegistry>::update();
        }

        void tst_ModelRegistry::setRegistries_data()
        {
            cleanTable();

            QTest::addColumn<std::vector<RegistryShared>>("objects");
            QTest::addColumn<bool>("result");

            QTest::newRow("exists rows in table") << helperSave() << true;
        }

        void tst_ModelRegistry::setRegistries()
        {
            QFETCH(std::vector<RegistryShared>, objects);
            QFETCH(bool, result);

            ModelRegistry modelRegistry;
            QCOMPARE(modelRegistry.setRegistries(), result);
            QCOMPARE(modelRegistry.m_objects.size(), objects.size());
            QCOMPARE(std::equal(dereference_iterator(modelRegistry.m_objects.begin()),
                       dereference_iterator(modelRegistry.m_objects.end()),
                       dereference_iterator(objects.begin())
                       ), true);
        }

        void tst_ModelRegistry::constructor()
        {
            const auto &objects = helperGetBaseRegistries();
            ModelRegistry modelRegistry;
            QCOMPARE(modelRegistry.m_objects.size(), objects.size());
            QCOMPARE(std::equal(dereference_iterator(modelRegistry.m_objects.begin()),
                       dereference_iterator(modelRegistry.m_objects.end()),
                       dereference_iterator(objects.begin())
                       ), true);
        }
    }

}

QTEST_MAIN(modules::tests::tst_ModelRegistry)

#include "tst_modelregistry.moc"
