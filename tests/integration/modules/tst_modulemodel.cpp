#include <QtTest>
#include "modulemodel.h"


namespace modules {

    namespace tests {

        class tst_ModuleModel : public QObject
        {
            Q_OBJECT
        public:
            tst_ModuleModel();
            ~tst_ModuleModel();

        private slots:
            void contructor();

        };

        tst_ModuleModel::tst_ModuleModel()
        {
        }

        tst_ModuleModel::~tst_ModuleModel()
        {

        }

        void tst_ModuleModel::contructor()
        {
            ModuleModel();
        }

    }

}

QTEST_APPLESS_MAIN(modules::tests::tst_ModuleModel)

#include "tst_modulemodel.moc"
