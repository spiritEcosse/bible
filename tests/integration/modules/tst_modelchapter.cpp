//
// Created by ihor on 12.12.2022.
//

#include "tst_modelchapter.h"
#include "dereferenceiterator.h"
#include "tst_modelverse.h"

namespace modules {
    namespace tests {

        void tst_ModelChapter::initTestCase() {
            BaseTest<Verse, ModelChapter, db::TranslationStorage>::initTestCase();
        }

        void tst_ModelChapter::cleanupTestCase() {
            BaseTest<Verse, ModelChapter, db::TranslationStorage>::cleanupTestCase();
        }

        std::vector<VerseUnique> tst_ModelChapter::helperGetObjectsUnique() const {
            std::vector<VerseUnique> objects;
            for(size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_unique<Verse>(in, in, in, in));
                objects.push_back(std::make_unique<Verse>(in, in, in + vectorSize, in + vectorSize));
                objects.push_back(std::make_unique<Verse>(in, in + vectorSize, in + vectorSize, in + vectorSize));
            }
            return objects;
        }

        // tests

        void tst_ModelChapter::updateObjects() {
            auto &&objects = helperSaveUnique();

            ModelChapter model(1);
            QCOMPARE(model.m_objects.size(), static_cast<size_t>(2));
            QCOMPARE(std::equal(dereference_iterator(model.m_objects.begin()),
                                dereference_iterator(model.m_objects.end()),
                                dereference_iterator(objects.begin() + 4)),
                     true);
            QCOMPARE(model.objectsCount, 0);
        }

        void tst_ModelChapter::roleNames() {
            ModelChapter model;
            QHash<int, QByteArray> data{{ModelChapter::BookNumber, "book_number"},
                                        {ModelChapter::Chapter, "chapter"},
                                        {ModelChapter::NumberVerses, "number_verses"},
                                        {ModelChapter::Verses, "verses"}};

            QCOMPARE(model.roleNames(), data);
        }
    }
}  // namespace modules
