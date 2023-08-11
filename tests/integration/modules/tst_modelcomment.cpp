//
// Created by ihor on 21.12.2022.
//

#include "tst_modelcomment.h"
#include "dereferenceiterator.h"
#include <QByteArray>

namespace modules {
    namespace tests {

        void tst_ModelComment::cleanup() {
            BaseTest<Comment, ModelComment, db::TranslationCommentStorage>::cleanup();
        }

        void tst_ModelComment::initTestCase() {
            BaseTest<Comment, ModelComment, db::TranslationCommentStorage>::initTestCase();
        }

        void tst_ModelComment::cleanupTestCase() {
            BaseTest<Comment, ModelComment, db::TranslationCommentStorage>::cleanupTestCase();
        }

        void tst_ModelComment::helperSaveStatic() {
            tst_ModelComment tst_model;
            tst_model.initDb();
            tst_model.helperSaveUnique();
        }

        std::vector<CommentUnique> tst_ModelComment::helperGetObjectsUnique() const {
            std::vector<CommentUnique> objects;
            for(size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_unique<Comment>(in, in, in, in, in, "", ""));
            }
            return objects;
        }

        // tests

        void tst_ModelComment::updateObjects() {
            auto &&objects = helperSaveUnique();

            ModelComment model(1, 1, 1);
            QCOMPARE(model.m_objects.size(), static_cast<size_t>(1));
            QCOMPARE(std::equal(dereference_iterator(model.m_objects.begin()),
                                dereference_iterator(model.m_objects.end()),
                                dereference_iterator(objects.begin() + 1)),
                     true);
            QCOMPARE(model.objectsCount, 1);
        }

        void tst_ModelComment::roleNames() {
            ModelComment model;
            QHash<int, QByteArray> data{{ModelComment::Text, "text"},
                                        {ModelComment::Marker, "marker"},
                                        {ModelComment::VerseNumberFrom, "verse_number_from"}};

            QCOMPARE(model.roleNames(), data);
        }
    }  // namespace tests
}  // namespace modules

#include "tst_modelcomment.moc"
