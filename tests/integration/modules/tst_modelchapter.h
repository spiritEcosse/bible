//
// Created by ihor on 12.12.2022.
//

#ifndef TST_MODELCHAPTER_H
#define TST_MODELCHAPTER_H

#include "basetest.h"
#include "modelchapter.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_ModelChapter : public ::tests::BaseTest<Verse, ModelChapter, db::TranslationStorage> {
            Q_OBJECT
        protected:
            [[nodiscard]] std::vector<VerseUnique> helperGetObjectsUnique() const override;
        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void updateObjects();
            void roleNames();
        };
    }  // namespace tests
}  // namespace modules

#endif  // TST_MODELCHAPTER_H
