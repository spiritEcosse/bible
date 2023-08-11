#ifndef TST_MANAGERGROUP_H
#define TST_MANAGERGROUP_H

#include "groupmodules.h"
#include "modeljsontest.h"
#include "modelregistry.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_ManagerGroup : public ::tests::ModelJsonTest<Registry, ModelRegistry> {
            Q_OBJECT

        public:
            tst_ManagerGroup();
            ~tst_ManagerGroup();

        private:
            QJsonDocument helperGetInvalidDocument() const;
            std::vector<RegistryUnique> helperGetObjectsUnique() const;
            std::vector<RegistryShared> helperGetObjects() const;

        private slots:
            void initTestCase();
            void cleanupTestCase();
            void makeCollections_data();
            void makeCollections();
        };
    }  // namespace tests
}  // namespace modules

#endif  // TST_MANAGERGROUP_H
