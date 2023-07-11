//
// Created by ihor on 17.12.2022.
//

#include "modelcomment.h"

namespace modules {

    ModelComment::ModelComment(QString &&fileName, QObject *parent) :
        ListModel<Comment, db::TranslationCommentStorage>(std::move(fileName), parent) {}

    ModelComment::ModelComment(int bookNumber,
                               int chapterNumber,
                               int verseNumber,
                               QString &&fileName,
                               QObject *parent) :
        ListModel<Comment, db::TranslationCommentStorage>(std::move(fileName), parent),
        m_bookNumber(bookNumber), m_chapterNumber(chapterNumber), m_verseNumber(verseNumber) {
        updateObjects();
    }

    void ModelComment::updateObjects() {
        beginResetModel();
        objectsCount = 0;
        m_objects = m_db->storage->get_all_pointer<Comment>(
            where(c(&Comment::m_bookNumber) == m_bookNumber and c(&Comment::m_chapterNumberFrom) == m_chapterNumber and
                  c(&Comment::m_verseNumberFrom) == m_verseNumber),
            order_by(&Comment::m_marker));
        objectsCount = static_cast<int>(m_objects.size());
        endResetModel();
    }

    // Override
    QHash<int, QByteArray> ModelComment::roleNames() const {
        return {{Text, "text"}, {Marker, "marker"}, {VerseNumberFrom, "verse_number_from"}};
    }

    QVariant ModelComment::data(const QModelIndex &index, int role) const {
        QVariant data{};

        if(!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &object = m_objects.at(index.row());

        switch(role) {
            case Text:
                data = object->m_text;
                break;
            case Marker:
                data = object->m_marker;
                break;
            case VerseNumberFrom:
                data = object->m_verseNumberFrom;
                break;
            default:
                data = QVariant();
        }

        return data;
    }
}  // namespace modules
