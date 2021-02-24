#include <QtTest>
#include "modelmodule.h"
#include "basetest.h"


namespace modules {

    namespace tests {

        class tst_ModelModule : public ::tests::BaseTest<Module, ModelModule> {
            Q_OBJECT

        public:
            tst_ModelModule();
            ~tst_ModelModule();

        private:
            std::vector<Module> helperGetObjects() const override;
        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void update() override;
        };

        tst_ModelModule::tst_ModelModule() {}

        tst_ModelModule::~tst_ModelModule() {}

        void tst_ModelModule::initTestCase()
        {
            ::tests::BaseTest<Module, ModelModule>::initTestCase();
        }

        void tst_ModelModule::cleanupTestCase()
        {
            ::tests::BaseTest<Module, ModelModule>::cleanupTestCase();
        }

        //helpers
        std::vector<Module> tst_ModelModule::helperGetObjects() const
        {
            return std::vector<Module> {
                vectorSize,
                {
                    "name",
                    "description",
                    "abbreviation",
                    0,
                    102400,
                    "en",
                    "information",
                    "comment",
                    "copyright",
                    QDate(2017, 03, 31),
                    true,
                    false
                }
            };
        }

        // tests
        void tst_ModelModule::update()
        {
            ::tests::BaseTest<Module, ModelModule>::update();
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelModule)

#include "tst_modelmodule.moc"
