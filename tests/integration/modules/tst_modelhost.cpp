#include <QtTest>
#include "modelhost.h"
#include "basetest.h"
#include "dereferenceiterator.h"

Q_DECLARE_METATYPE(modules::HostShared)
Q_DECLARE_METATYPE(std::vector<modules::HostShared>)

namespace modules {

    namespace tests {

        class tst_ModelHost : public ::tests::BaseTest<Host, ModelHost>  {
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
        };

        tst_ModelHost::tst_ModelHost() {}

        tst_ModelHost::~tst_ModelHost() {}

        void tst_ModelHost::initTestCase()
        {
            ::tests::BaseTest<Host, ModelHost>::initTestCase();
        }

        void tst_ModelHost::cleanupTestCase()
        {
            ::tests::BaseTest<Host, ModelHost>::cleanupTestCase();
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

        void tst_ModelHost::update()
        {
            ::tests::BaseTest<Host, ModelHost>::update();
        }

        void tst_ModelHost::transform_data()
        {
            ::tests::BaseTest<Host, ModelHost>::transform_data();
        }

        void tst_ModelHost::transform()
        {
            ::tests::BaseTest<Host, ModelHost>::transform();
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelHost)

#include "tst_modelhost.moc"
