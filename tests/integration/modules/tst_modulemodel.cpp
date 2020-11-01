#include <QtTest>
#include "modelmodule.h"


namespace modules {

    namespace tests {

        class tst_ModelModule: public QObject
        {
            Q_OBJECT
        public:
            tst_ModelModule();
            ~tst_ModelModule();

        };

        tst_ModelModule::tst_ModelModule()
        {
        }

        tst_ModelModule::~tst_ModelModule()
        {

        }

    }

}

QTEST_APPLESS_MAIN(modules::tests::tst_ModelModule)

#include "tst_modulemodel.moc"
