//
// Created by ihor on 21.11.2022.
//

#include "modelverse.h"
#include "modelcomment.h"

namespace modules {

    ModelVerse::ModelVerse(QString &&fileName, QObject *parent) :
        ListModel<Verse, db::TranslationStorage>(std::move(fileName), parent) {}

    ModelVerse::ModelVerse(std::shared_ptr<QString> searchText, int bookNumber, QString &&fileName, QObject *parent) :
        ListModel<Verse, db::TranslationStorage>(std::move(fileName), parent), m_searchText(std::move(searchText)),
        m_bookNumber(bookNumber) {
        searchVersesByText();
    }

    ModelVerse::ModelVerse(int bookNumber, int chapterNumber, QString &&fileName, QObject *parent) :
        ListModel<Verse, db::TranslationStorage>(std::move(fileName), parent), m_bookNumber(bookNumber),
        m_chapterNumber(chapterNumber) {
        updateObjects();
    }

    void ModelVerse::updateObjects() {
        beginResetModel();
        objectsCount = 0;
        m_objects = m_db->storage->get_all_pointer<Verse>(
            where(c(&Verse::m_bookNumber) == m_bookNumber and c(&Verse::m_chapter) == m_chapterNumber),
            order_by(&Verse::m_verse));
        endResetModel();
    }

    void ModelVerse::searchVersesByText() {
        try {
            // Reset the model before searching
            beginResetModel();
            // Reset the count of objects
            objectsCount = 0;
            // Get all Verse objects from storage that match the specified conditions
            m_objects = m_db->storage->get_all_pointer<Verse>(
                where(c(&Verse::m_bookNumber) == m_bookNumber and like(&Verse::m_text, *m_searchText + "%")),
                order_by(&Verse::m_verse));
            // Notify the model that the reset is complete
            endResetModel();
        } catch(const std::system_error &e) {
            // Log the error and emit an error signal
            qCritical() << "Error searching verses in ModelVerse: " << e.what();
            emit error("An error occurred while searching verses.");
        } catch(...) {
            throw;
        }
    }

    // Override
    QHash<int, QByteArray> ModelVerse::roleNames() const {
        return {
            {BookNumber, "book_number"},
            {Chapter, "chapter"},
            {NumberVerse, "verse"},
            {Text, "content_text"},
            {Comments, "comments"},
        };
    }

    QVariant ModelVerse::data(const QModelIndex &index, int role) const {
        QVariant data{};

        if(!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &object = m_objects.at(index.row());

        switch(role) {
            case BookNumber:
                data = object->m_bookNumber;
                break;
            case Chapter:
                data = object->m_chapter;
                break;
            case NumberVerse:
                data = object->m_verse;
                break;
            case Text:
                data = object->m_text;
                break;
            case Comments:
                if(object->m_comments == nullptr) {
                    object->m_comments = std::make_shared<ModelComment>(object->m_bookNumber,
                                                                        object->m_chapter,
                                                                        object->m_verse,
                                                                        std::move(m_db->getFullPathDbComment()));
                }

#ifdef Qt6_FOUND
                data = QVariant::fromValue(object->m_comments.get());
#else
                data = qVariantFromValue(object->m_comments.get());
#endif
                break;
            default:
                data = QVariant();
        }

        return data;
    }
}  // namespace modules
