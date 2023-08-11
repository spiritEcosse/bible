//
// Created by ihor on 20.12.2022.
//

#ifndef TST_MODELRECORD_H
#define TST_MODELRECORD_H

#include "basetest.h"
#include "modelrecord.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_ModelRecord : public ::tests::BaseTest<Record, ModelRecord> {
            Q_OBJECT
        public:
            static void helperSaveStatic();

        protected:
            [[nodiscard]] std::vector<RecordUnique> helperGetObjectsUnique() const override;

        private slots:
            void cleanup() override;
            void initTestCase() override;
            void cleanupTestCase() override;
            void updateObjects();
            void createRecord_data();
            void createRecord();
            void roleNames();
        };
    }  // namespace tests
}  // namespace modules

#endif  //TST_MODELRECORD_H
