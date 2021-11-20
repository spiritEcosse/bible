#include <QtTest>
#include "modelhost.h"
#include "modeljsontest.h"
#include "dereferenceiterator.h"

namespace modules {

    namespace tests {

        class tst_ModelHost : public ::tests::ModelJsonTest<Host, ModelHost>  {
            Q_OBJECT

        private:
            std::vector<HostShared> helperGetObjects() const override;
            std::vector<HostUnique> helperGetObjectsUnique() const override;

        public:
            tst_ModelHost();
            ~tst_ModelHost();

        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void update() override;
            void transform_data() override;
            void transform() override;
            void populateStaticObjects();
        };

        tst_ModelHost::tst_ModelHost() {}

        tst_ModelHost::~tst_ModelHost() {}

        void tst_ModelHost::initTestCase()
        {
            ModelJsonTest<Host, ModelHost>::initTestCase();
        }

        void tst_ModelHost::cleanupTestCase()
        {
            ModelJsonTest<Host, ModelHost>::cleanupTestCase();
        }

        //helpers

        std::vector<HostShared> tst_ModelHost::helperGetObjects() const
        {
            std::vector<HostShared> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_shared<Host>("alias", "bGluazEx", 1, 2));
            }
            return objects;
        }

        std::vector<HostUnique> tst_ModelHost::helperGetObjectsUnique() const
        {
            std::vector<HostUnique> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_unique<Host>("alias", "bGluazEx", 1, 2));
            }
            return objects;
        }

        // tests


        void tst_ModelHost::populateStaticObjects()
        {
            cleanTable();
            helperSave();

            const auto &objects = helperGetObjectsUnique();
            ModelHost model;
//            model.populateStaticObjects();
//            QCOMPARE(model.objectsStatic.size(), objects.size());
//            QCOMPARE(std::equal(dereference_iterator(model.objectsStatic.begin()),
//                       dereference_iterator(model.objectsStatic.end()),
//                       dereference_iterator(objects.begin())
//                       ), true);
        }

        void tst_ModelHost::update()
        {
            ModelJsonTest<Host, ModelHost>::update();
        }

        void tst_ModelHost::transform_data()
        {
            ModelJsonTest<Host, ModelHost>::transform_data();
        }

        void tst_ModelHost::transform()
        {
            ModelJsonTest<Host, ModelHost>::transform();
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelHost)

#include "tst_modelhost.moc"
