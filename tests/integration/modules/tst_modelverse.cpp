//
// Created by ihor on 02.12.2022.
//

#include "tst_modelverse.h"
#include "tst_modelbook.h"

#include "dereferenceiterator.h"

namespace modules {
    namespace tests {

        void tst_ModelVerse::initTestCase() {
            BaseTest<Verse, ModelVerse, db::TranslationStorage>::initTestCase();
        }

        void tst_ModelVerse::cleanupTestCase() {
            BaseTest<Verse, ModelVerse, db::TranslationStorage>::cleanupTestCase();
        }

        void tst_ModelVerse::helperSaveStatic() {
            tst_ModelVerse tst_model;
            tst_model.initDb();
            tst_model.helperSaveUnique();
        }

        std::vector<VerseUnique> tst_ModelVerse::helperGetObjectsUnique() const {
            std::vector<VerseUnique> objects;
            for(size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_unique<Verse>(in, in, in, QString("text.%1").arg(in)));
            }
            return objects;
        }

        // tests

        void tst_ModelVerse::updateObjects() {
            cleanTable();
            auto &&objects = helperSaveUnique();

            ModelVerse model(1, 1);
            QCOMPARE(model.m_objects.size(), static_cast<size_t>(1));
            QCOMPARE(std::equal(dereference_iterator(model.m_objects.begin()),
                                dereference_iterator(model.m_objects.end()),
                                dereference_iterator(objects.begin() + 1)),
                     true);
            QCOMPARE(model.objectsCount, 0);
        }

        void tst_ModelVerse::roleNames() {
            ModelVerse model;
            QHash<int, QByteArray> data{
                {ModelVerse::BookNumber, "book_number"},
                {ModelVerse::Chapter, "chapter"},
                {ModelVerse::NumberVerse, "verse"},
                {ModelVerse::Text, "content_text"},
                {ModelVerse::Comments, "comments"},
            };

            QCOMPARE(model.roleNames(), data);
        }

        void tst_ModelVerse::searchVersesByText() {
            cleanTable();
            tst_ModelBook::helperSaveStatic();
            auto &&objects = helperSaveUnique();

            ModelVerse model(std::make_shared<QString>("text.1"), 1);
            QCOMPARE(model.m_objects.size(), static_cast<size_t>(1));
            QCOMPARE(std::equal(dereference_iterator(model.m_objects.begin()),
                                dereference_iterator(model.m_objects.end()),
                                dereference_iterator(objects.begin() + 1)),
                     true);
            QCOMPARE(model.objectsCount, 0);
        }
    }  // namespace tests
}  // namespace modules

#include "tst_modelverse.moc"
