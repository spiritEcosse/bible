#ifndef TST_MODELREGISTRY_H
#define TST_MODELREGISTRY_H

#include "modeljsontest.h"
#include "modelregistry.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_ModelRegistry : public ::tests::ModelJsonTest<Registry, ModelRegistry> {
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
            void transform_data() override;
            void transform() override;
            void setRegistries_data();
            void setRegistries();
            void constructor();
        };
    }  // namespace tests
}  // namespace modules

#endif  // TST_MODELREGISTRY_H
