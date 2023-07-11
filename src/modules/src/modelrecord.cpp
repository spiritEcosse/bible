//
// Created by ihor on 20.12.2022.
//

#include "modelrecord.h"
#include <QtQuick>

namespace modules {

    ModelRecord::ModelRecord() : ListModel<Record>() {
        updateObjects();
    }

    void ModelRecord::updateObjects() {
        beginResetModel();
        objectsCount = 0;
        m_objects = m_db->storage->get_all_pointer<Record>(order_by(&Record::m_timestamp).desc());

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

    void ModelRecord::createRecord(QString bookShortName,
                                   int bookIndex,
                                   int chapterIndex,
                                   int verseIndex,
                                   QDateTime timestamp) {
        if(!m_objects.empty()) {
            const auto &object = m_objects[0];

            if ( bookIndex == object->m_bookIndex && chapterIndex == object->m_chapterIndex &&
               verseIndex == object->m_verseIndex ) {
                return;
            }
        }
        m_db->storage->insert(
            Record(std::move(bookShortName), bookIndex, chapterIndex, verseIndex, std::move(timestamp)));
        updateObjects();
    }

    int ModelRecord::bookIndex() const
    {
        return m_bookIndex;
    }

    int ModelRecord::chapterIndex() const
    {
        return m_chapterIndex;
    }

    int ModelRecord::verseIndex() const
    {
        return m_verseIndex;
    }

    void ModelRecord::setBookIndex(int bookIndex)
    {
        if (bookIndex == m_bookIndex) {
            return;
        }

        m_bookIndex = bookIndex;
        m_chapterIndex = -1;
        m_verseIndex = -1;
        emit changeBookIndex();
        emit changeChapterIndex();
        emit changeVerseIndex();
    }

    void ModelRecord::setChapterIndex(int chapterIndex)
    {
        if (chapterIndex == m_chapterIndex)
        {
            return;
        }

        m_chapterIndex = chapterIndex;
        m_verseIndex = -1;
        emit changeChapterIndex();
        emit changeVerseIndex();
    }

    void ModelRecord::setVerseIndex(int verseIndex)
    {
        if (verseIndex == m_verseIndex)
        {
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
