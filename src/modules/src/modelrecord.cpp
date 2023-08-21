//
// Created by ihor on 20.12.2022.
//

#include "modelrecord.h"
#include <QtQuick>
#include <sqlite3.h>
#include "modelmodule.h"

namespace modules {

    ModelRecord::ModelRecord() : ListModel<Record>(), m_modelModule(new ModelModule()) {
        int rc = sqlite3_open(m_db->getFileName().toLocal8Bit().data(), &db);

        if(rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        } else {
            qDebug() << m_db->getFileName().toLocal8Bit().data() << "Opened database successfully";
        }
    }

    ModelRecord::~ModelRecord() noexcept {
        sqlite3_close(db);
    }

    void ModelRecord::updateObjects() {
        QString dbModule = std::move(m_modelModule->getPathDbActiveModule());

        if(!dbModule.isEmpty()) {
            int rc;
            // Attach another database
            rc = sqlite3_exec(db,
                              QString("ATTACH DATABASE \"%1\" AS attached").arg(dbModule).toLocal8Bit().data(),
                              nullptr,
                              nullptr,
                              nullptr);
            if(rc != SQLITE_OK) {
                qCritical() << "Cannot attach database: " << sqlite3_errmsg(db);
            } else {
                qDebug() << "Database is attached";

                const char *sql = "SELECT atb.short_name, mr.* FROM record AS mr INNER JOIN books_all AS atb ON "
                                  "(mr.book_number==atb.book_number) ORDER BY mr.timestamp DESC";

                sqlite3_stmt *stmt;
                rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                if(rc != SQLITE_OK) {
                    qCritical() << "SELECT failed: " << sqlite3_errmsg(db);
                } else {
                    beginResetModel();
                    objectsCount = 0;
                    m_objects.clear();

                    while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                        const char *bookShortName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
                        const char *timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
                        const int bookNumber = reinterpret_cast<const int>(sqlite3_column_int(stmt, 2));
                        const int bookIndex = reinterpret_cast<const int>(sqlite3_column_int(stmt, 3));
                        int chapterIndex = reinterpret_cast<const int>(sqlite3_column_int(stmt, 4));
                        int verseIndex = reinterpret_cast<const int>(sqlite3_column_int(stmt, 5));
                        m_objects.push_back(
                            std::make_unique<Record>(bookNumber,
                                                     bookIndex,
                                                     chapterIndex,
                                                     verseIndex,
                                                     QDateTime::fromString(timestamp, ISODateTimeFormat),
                                                     bookShortName));
                    }

                    if(rc != SQLITE_DONE) {
                        qCritical() << "SELECT failed: " << sqlite3_errmsg(db);
                    }

                    if(!m_objects.empty()) {
                        m_bookIndex = m_objects[0]->m_bookIndex;
                        m_chapterIndex = m_objects[0]->m_chapterIndex;
                        m_verseIndex = m_objects[0]->m_verseIndex;

                        emit changeBookIndex();
                        emit changeChapterIndex();
                        emit changeVerseIndex();
                    }
                    endResetModel();
                }
                sqlite3_finalize(stmt);
            }

            // Detach the attached database
            rc = sqlite3_exec(db, "DETACH DATABASE attached;", nullptr, nullptr, nullptr);
            if(rc != SQLITE_OK) {
                qCritical() << "Cannot detach database: " << sqlite3_errmsg(db);
            }
        }
    }

    void
    ModelRecord::createRecord(int bookNumber, int bookIndex, int chapterIndex, int verseIndex, QDateTime timestamp) {
        if(!m_objects.empty()) {
            const auto &object = m_objects[0];

            if(bookIndex == object->m_bookIndex && chapterIndex == object->m_chapterIndex &&
               verseIndex == object->m_verseIndex) {
                return;
            }
        }
        m_db->storage->insert(Record(bookNumber, bookIndex, chapterIndex, verseIndex, std::move(timestamp)));
        updateObjects();
    }

    int ModelRecord::bookIndex() const {
        return m_bookIndex;
    }

    int ModelRecord::chapterIndex() const {
        return m_chapterIndex;
    }

    int ModelRecord::verseIndex() const {
        return m_verseIndex;
    }

    void ModelRecord::setBookIndex(int bookIndex) {
        if(bookIndex == m_bookIndex) {
            return;
        }

        m_bookIndex = bookIndex;
        m_chapterIndex = -1;
        m_verseIndex = -1;
        emit changeBookIndex();
        emit changeChapterIndex();
        emit changeVerseIndex();
    }

    void ModelRecord::setChapterIndex(int chapterIndex) {
        if(chapterIndex == m_chapterIndex) {
            return;
        }

        m_chapterIndex = chapterIndex;
        m_verseIndex = -1;
        emit changeChapterIndex();
        emit changeVerseIndex();
    }

    void ModelRecord::setVerseIndex(int verseIndex) {
        if(verseIndex == m_verseIndex) {
            return;
        }

        m_verseIndex = verseIndex;
        emit changeVerseIndex();
    }

    void ModelRecord::registerMe() {
        qmlRegisterType<ModelRecord>("bible.ModelRecord", 1, 0, "ModelRecord");
    }

    // Override
    QHash<int, QByteArray> ModelRecord::roleNames() const {
        return {{Timestamp, "timestamp"},
                {BookNumber, "book_number"},
                {BookShortName, "book_short_name"},
                {BookIndex, "book_index"},
                {ChapterIndex, "chapter_index"},
                {VerseIndex, "verse_index"}};
    }

    QVariant ModelRecord::data(const QModelIndex &index, int role) const {
        QVariant data{};

        if(!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &object = m_objects.at(index.row());

        switch(role) {
            case Timestamp:
                data = object->m_timestamp;
                break;
            case BookNumber:
                data = object->m_bookNumber;
                break;
            case BookShortName:
                data = object->m_bookShortName;
                break;
            case BookIndex:
                data = object->m_bookIndex;
                break;
            case ChapterIndex:
                data = object->m_chapterIndex;
                break;
            case VerseIndex:
                data = object->m_verseIndex;
                break;
            default:
                data = QVariant();
        }

        return data;
    }

}  // namespace modules
