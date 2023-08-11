//
// Created by ihor on 21.12.2022.
//

#ifndef TST_MODELCOMMENT_H
#define TST_MODELCOMMENT_H

#include "basetest.h"
#include "modelcomment.h"
#include <QtTest>

namespace modules {
    namespace tests {

        class tst_ModelComment : public ::tests::BaseTest<Comment, ModelComment, db::TranslationCommentStorage> {
            Q_OBJECT
        public:
            ~tst_ModelComment() override = default;
            static void helperSaveStatic();

        protected:
            [[nodiscard]] std::vector<CommentUnique> helperGetObjectsUnique() const override;

        private slots:
            void cleanup() override;
            void initTestCase() override;
            void cleanupTestCase() override;
            void updateObjects();
            static void roleNames();
        };
    }  // namespace tests
}  // namespace modules

#endif  //TST_MODELCOMMENT_H
