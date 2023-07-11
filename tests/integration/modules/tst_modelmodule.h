#ifndef TST_MODELMODULE_H
#define TST_MODELMODULE_H

#include "modeljsontest.h"
#include "modelmodule.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_ModelModule : public ::tests::ModelJsonTest<Module, ModelModule> {
            Q_OBJECT

          public:
            tst_ModelModule();
            ~tst_ModelModule();

            static void helperSaveStaticAndSetExtraFieldsTrue();
            static void helperCheckAllData(const std::vector<ModelShared> &modules);

          private:
            std::vector<ModuleShared> helperGetObjects() const override;
            std::vector<ModuleUnique> helperGetObjectsUnique() const override;
            std::vector<HostUnique> helperGetHostsUnique();
            HostUnique helperGetHostsUniqueNotExists() const;
            HostUnique helperGetHostUnique() const;
            QVariantList helperGetSelected() const;

            int m_idGroupModules = 0;
            bool m_downloaded = false;
          private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void update() override;
            void init_model();
            void constructor_params();
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
}  // namespace modules

#endif  // TST_MODELMODULE_H
