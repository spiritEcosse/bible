//
// Created by ihor on 28.11.2022.
//

#include "tst_modelbook.h"
#include "dereferenceiterator.h"
#include "tst_modelverse.h"

namespace modules {
    namespace tests {

        void tst_ModelBook::initTestCase() {
            BaseTest<Book, ModelBook, db::TranslationStorage>::initTestCase();
        }

        void tst_ModelBook::cleanupTestCase() {
            BaseTest<Book, ModelBook, db::TranslationStorage>::cleanupTestCase();
        }

        // helpers
        void tst_ModelBook::helperSaveStatic() {
            tst_ModelBook tst_modelBook;
            tst_modelBook.initDb();
            tst_modelBook.helperSave();
        }

        std::vector<BookUnique> tst_ModelBook::helperGetObjectsUnique() const {
            std::vector<BookUnique> objects;
            for(size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_unique<Book>(in,
                                                         QString("shortName.%1").arg(in),
                                                         QString("longName.%1").arg(in),
                                                         QString("bookColor.%1").arg(in),
                                                         false,
                                                         in));
            }
            return objects;
        }

        // tests

        void tst_ModelBook::constructor() {
            tst_ModelVerse::helperSaveStatic();
            auto &&objects = helperSaveUnique();

            ModelBook model;

            QCOMPARE(model.m_objects.size(), objects.size());
            QCOMPARE(std::equal(dereference_iterator(model.m_objects.begin()),
                                dereference_iterator(model.m_objects.end()),
                                dereference_iterator(objects.begin())),
                     true);
            int pos = 0;
            for(BookUnique &book: model.m_objects) {
                QCOMPARE(*book, *objects[pos]);
                QCOMPARE(book->m_numberChapters, objects[pos]->m_numberChapters);
                pos++;
            }
            QCOMPARE(model.objectsCount, 0);
        }

        void tst_ModelBook::roleNames() {
            ModelBook model;
            QHash<int, QByteArray> data{{ModelBook::BookNumber, "book_number"},
                                        {ModelBook::ShortName, "short_name"},
                                        {ModelBook::LongName, "long_name"},
                                        {ModelBook::BookColor, "book_color"},
                                        {ModelBook::IsPresent, "is_present"},
                                        {ModelBook::Chapters, "chapters"},
                                        {ModelBook::NumberChapters, "number_chapters"},
                                        {ModelBook::FoundVerses, "foundVerses"}};

            QCOMPARE(model.roleNames(), data);
        }

        void tst_ModelBook::testSearchVersesByText_data() {
            QTest::addColumn<QString>("searchQueryInVerseText");
            QTest::addColumn<int>("count");
            QTest::addColumn<int>("startPosition");
            QTest::addColumn<bool>("hitDb");
            QTest::newRow("empty searchQueryInVerseText") << "" << 0 << 0 << false;
            QTest::newRow("not empty searchQueryInVerseText") << "text.1" << 1 << 1 << true;
            QTest::newRow("several results") << "text" << static_cast<int>(vectorSize) << 0 << true;
        }

        void tst_ModelBook::testSearchVersesByText() {
            QFETCH(QString, searchQueryInVerseText);
            QFETCH(int, count);
            QFETCH(int, startPosition);
            QFETCH(bool, hitDb);

            tst_ModelVerse::helperSaveStatic();
            auto &&objects = helperSaveUnique();

            ModelBook model("", true);
            QSignalSpy spy(model.m_queryTimer.get(), &QTimer::timeout);
            model.m_waitingTimeBeforeHitDb = 0;

            model.searchVersesByText(searchQueryInVerseText);
            if(hitDb) {
                QVERIFY(spy.wait());
                QCOMPARE(spy.count(), 1);
            }

            QCOMPARE(int(model.m_objects.size()), count);
            QCOMPARE(std::equal(dereference_iterator(model.m_objects.begin()),
                                dereference_iterator(model.m_objects.end()),
                                dereference_iterator(objects.begin() + startPosition)),
                     true);
            QCOMPARE(model.objectsCount, 0);
        }
    }
}

#include "tst_modelbook.moc"
