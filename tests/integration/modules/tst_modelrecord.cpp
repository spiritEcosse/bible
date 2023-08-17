//
// Created by ihor on 20.12.2022.
//

#include "tst_modelrecord.h"
#include <algorithm>
#include <memory>
#include "tst_modelmodule.h"
#include "tst_modelbook.h"

#include "dereferenceiterator.h"

Q_DECLARE_METATYPE(std::vector<modules::RecordShared>)

namespace modules {
    namespace tests {

        // I have to override this method because I need to attach databases user.sqlite and '/<module>/.SQLite3'
        // and read data in ModelRecord::updateObjects.
        // TODO: remove it and attach databases in memory
        void tst_ModelRecord::initDb() {
            m_db = std::make_unique<db::Db<Record>>("");  // fileName "" will take user.sqlite
            cleanTable();
        }

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
                objects.push_back(std::make_unique<Record>(in,
                                                           in,
                                                           in,
                                                           in,
                                                           QDateTime::currentDateTime(),
                                                           QString("shortName.%1").arg(in)));
            }
            return objects;
        }

        // tests

        void tst_ModelRecord::updateObjects() {
            tst_ModelModule::helperSaveStaticAndSetExtraFieldsTrue();
            tst_ModelBook::helperSaveStatic();
            auto &&objects = helperSaveUnique();

            ModelRecord model;
            QSignalSpy spyChangeBookIndex(&model, &ModelRecord::changeBookIndex);
            QSignalSpy spyChangeChapterIndex(&model, &ModelRecord::changeChapterIndex);
            QSignalSpy spyChangeVerseIndex(&model, &ModelRecord::changeVerseIndex);
            model.updateObjects();
            QCOMPARE(spyChangeBookIndex.count(), 1);
            QCOMPARE(spyChangeChapterIndex.count(), 1);
            QCOMPARE(spyChangeVerseIndex.count(), 1);
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
            int index = 1;
            std::vector<RecordShared> records{std::make_shared<Record>(index,
                                                                       index,
                                                                       index,
                                                                       index,
                                                                       QDateTime::currentDateTime(),
                                                                       QString("shortName.%1").arg(index))};

            QTest::newRow("success make insert") << records;
            records.push_back(std::make_shared<Record>(index, index, index, index, QDateTime::currentDateTime()));
            QTest::newRow("not success make insert, cause duplicate") << records;
        }

        void tst_ModelRecord::createRecord() {
            QFETCH(std::vector<RecordShared>, objects);

            ModelRecord model;

            QCOMPARE(model.m_objects.size(), static_cast<size_t>(0));
            std::for_each(objects.begin(), objects.end(), [&model](const auto &object) {
                model.createRecord(object->m_bookNumber,
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
                                        {ModelRecord::BookNumber, "book_number"},
                                        {ModelRecord::BookShortName, "book_short_name"},
                                        {ModelRecord::BookIndex, "book_index"},
                                        {ModelRecord::ChapterIndex, "chapter_index"},
                                        {ModelRecord::VerseIndex, "verse_index"}};

            QCOMPARE(model.roleNames(), data);
        }
    }  // namespace tests
}  // namespace modules

#include "tst_modelrecord.moc"
