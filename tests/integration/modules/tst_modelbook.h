//
// Created by ihor on 28.11.2022.
//

#ifndef TST_MODELBOOK_H
#define TST_MODELBOOK_H

#include "basetest.h"
#include "modelbook.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_ModelBook : public ::tests::BaseTest<Book, ModelBook, db::TranslationStorage> {
            Q_OBJECT
          public:
            static void helperSaveStatic();

          protected:
            [[nodiscard]] std::vector<BookUnique> helperGetObjectsUnique() const override;

          private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void constructor();
            void roleNames();
        };
    }
}

#endif  // TST_MODELBOOK_H
