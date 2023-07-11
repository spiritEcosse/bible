//
// Created by ihor on 02.12.2022.
//

#ifndef TST_MODELVERSE_H
#define TST_MODELVERSE_H

#include "basetest.h"
#include "modelverse.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_ModelVerse : public ::tests::BaseTest<Verse, ModelVerse, db::TranslationStorage> {
            Q_OBJECT
          public:
            static void helperSaveStatic();

          protected:
            [[nodiscard]] std::vector<VerseUnique> helperGetObjectsUnique() const override;

          private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void updateObjects();
            void roleNames();
        };
    }
}  // namespace modules

#endif  // TST_MODELVERSE_H
