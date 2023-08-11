#ifndef TST_MODELHOST_H
#define TST_MODELHOST_H

#include "modelhost.h"
#include "modeljsontest.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_ModelHost : public ::tests::ModelJsonTest<Host, ModelHost> {
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
    }  // namespace tests
}  // namespace modules

#endif  // TST_MODELHOST_H
