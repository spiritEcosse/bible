#ifndef TST_MODELGROUPMODULES_H
#define TST_MODELGROUPMODULES_H

#include <QtTest>
#include "modelgroupmodules.h"
#include "modeljsontest.h"

namespace modules {

    namespace tests {

        class tst_ModelGroupModules : public ::tests::ModelJsonTest<GroupModules, ModelGroupModules>
        {
            Q_OBJECT
        public:
            tst_ModelGroupModules();
            ~tst_ModelGroupModules();

            static void helperSaveStatic();

        private:
            std::vector<GroupModulesShared> helperGetObjects() const override;
            void setQSettings(int value = 0, QString key = "registryVersion");
            std::vector<GroupModulesUnique> helperGetObjectsUnique() const override;

        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void contructor_data();
            void contructor();
            void newVersionAvailable();
            void updateCompleted();
            void update() override;
            void downloadRegistry_data();
            void downloadRegistry();
            void updateObjects_data();
            void updateObjects();
            void search_data();
            void search();
            void getAll_data();
            void getAll();
            void setFieldSearch();
            void setFieldSearch_data();
        };
    }
}

#endif // TST_MODELGROUPMODULES_H
