#ifndef TST_MODELMODULE_H
#define TST_MODELMODULE_H

#include <QtTest>
#include "modelmodule.h"
#include "modeljsontest.h"

namespace modules {

    namespace tests {

        class tst_ModelModule : public ::tests::ModelJsonTest<Module, ModelModule> {
            Q_OBJECT

        public:
            tst_ModelModule();
            ~tst_ModelModule();

        private:
            std::vector<ModuleShared> helperGetObjects() const override;
            std::vector<ModuleUnique> helperGetObjectsUnique() const override;
            std::vector<HostUnique> helperGetHostsUnique();
            HostUnique helperGetHostsUniqueNotExists() const;
            HostUnique helperGetHostUnique() const;
            QVariantList helperGetSelected() const;
        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void update() override;
            void init_model();
            void updateObjects_data();
            void updateObjects();
            void updateObjectsDownloaded();
            void updateSelected_data();
            void updateSelected();
            void updateDownloaded_data();
            void updateDownloaded();
            void getExtraFieldsFromDb();
            void saveExtraFieldsToDb();
            void saveExtraFieldsToDb_data();
            void downloadModules_data();
            void downloadModules();
            void downloadModules_withoutRecursion();
            void deleteModules();
            void retrieveDownloaded();
            void retrieveSelected();
        };
    }
}

#endif // TST_MODELMODULE_H
