//
// Created by ihor on 20.12.2022.
//

#include "tst_modelrecord.h"
#include <algorithm>

#include "dereferenceiterator.h"

Q_DECLARE_METATYPE(std::vector<modules::RecordShared>)

namespace modules {
    namespace tests {

        void tst_ModelRecord::cleanup() {
            BaseTest<Record, ModelRecord>::cleanup();
        }

        void tst_ModelRecord::initTestCase() {
            BaseTest<Record, ModelRecord>::initTestCase();
        }

        void tst_ModelRecord::cleanupTestCase() {
            BaseTest<Record, ModelRecord>::cleanupTestCase();
        }

        void tst_ModelRecord::helperSaveStatic() {
            tst_ModelRecord tst_model;
            tst_model.initDb();
            tst_model.helperSaveUnique();
        }

        std::vector<RecordUnique> tst_ModelRecord::helperGetObjectsUnique() const {
            std::vector<RecordUnique> objects;
            for(size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_unique<Record>("", in, in, in, QDateTime::currentDateTime()));
            }
            return objects;
        }

        // tests

        void tst_ModelRecord::updateObjects() {
            auto &&objects = helperSaveUnique();

            ModelRecord model;
            QCOMPARE(model.m_objects.size(), vectorSize);
            std::sort(objects.begin(), objects.end(), [](const auto &x, const auto &y) {
                return (x->m_timestamp > y->m_timestamp);
            });
            QCOMPARE(std::equal(dereference_iterator(model.m_objects.begin()),
                                dereference_iterator(model.m_objects.end()),
                                dereference_iterator(objects.begin())),
                     true);
            QCOMPARE(model.objectsCount, 0);
        }

        void tst_ModelRecord::createRecord_data() {
            QTest::addColumn<std::vector<RecordShared>>("objects");
            std::vector<RecordShared> records{std::make_shared<Record>("", 1, 1, 1, QDateTime::currentDateTime())};

            QTest::newRow("success make insert") << records;
            records.push_back(std::make_shared<Record>("", 1, 1, 1, QDateTime::currentDateTime()));
            QTest::newRow("not success make insert, cause duplicate") << records;
        }

        void tst_ModelRecord::createRecord() {
            QFETCH(std::vector<RecordShared>, objects);

            ModelRecord model;

            QCOMPARE(model.m_objects.size(), static_cast<size_t>(0));
            std::for_each(objects.begin(), objects.end(), [&model](const auto &object) {
                model.createRecord(object->m_bookShortName,
                                   object->m_bookIndex,
                                   object->m_chapterIndex,
                                   object->m_verseIndex,
                                   object->m_timestamp);
            });
            QCOMPARE(m_db->count(), 1);
            QCOMPARE(model.m_objects.size(), static_cast<size_t>(1));
            QCOMPARE(std::equal(dereference_iterator(model.m_objects.begin()),
                                dereference_iterator(model.m_objects.end()),
                                dereference_iterator(objects.begin())),
                     true);
            QCOMPARE(model.objectsCount, 0);
        }

        void tst_ModelRecord::roleNames() {
            ModelRecord model;
            QHash<int, QByteArray> data{{ModelRecord::Timestamp, "timestamp"},
                                        {ModelRecord::BookShortName, "book_short_name"},
                                        {ModelRecord::BookIndex, "book_index"},
                                        {ModelRecord::ChapterIndex, "chapter_index"},
                                        {ModelRecord::VerseIndex, "verse_index"}};

            QCOMPARE(model.roleNames(), data);
        }
    }
}  // namespace modules

#include "tst_modelrecord.moc"
