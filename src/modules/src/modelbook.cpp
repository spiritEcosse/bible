//
// Created by ihor on 21.11.2022.
//

#include "modelbook.h"
#include "modelchapter.h"
#include "modelverse.h"
#include <iostream>
#include <memory>
#include <QtQuick>

int MIN_LENGTH_SEARCH_QUERY = 2;

namespace modules {

    ModelBook::ModelBook(QString &&fileName, [[maybe_unused]] bool search, QObject *parent) :
        ListModel<Book, db::TranslationStorage>(std::move(fileName), parent) {
        if(search) {
            m_queryTimer = std::make_unique<QTimer>(this);
            m_queryTimer->setSingleShot(true);
            connect(m_queryTimer.get(), &QTimer::timeout, this, &ModelBook::doSearchVersesByText);
        } else {
            updateObjects();
        }
    }

    void ModelBook::updateObjects() {
        beginResetModel();
        objectsCount = 0;
        m_objects.clear();
        auto &&data = m_db->storage->select(columns(rowid<Book>(),
                                                    &Book::m_bookNumber,
                                                    &Book::m_shortName,
                                                    &Book::m_longName,
                                                    &Book::m_bookColor,
                                                    sqlite_orm::max(&Verse::m_chapter)),
                                            inner_join<Verse>(on(c(&Book::m_bookNumber) == &Verse::m_bookNumber)),
                                            group_by(&Book::m_bookNumber),
                                            order_by(&Book::m_bookNumber));

        for(auto &&book: data) {
            m_objects.push_back(std::make_unique<Book>(std::get<1>(book),
                                                       std::move(std::get<2>(book)),
                                                       std::move(std::get<3>(book)),
                                                       std::move(std::get<4>(book)),
                                                       *std::get<5>(book).get(),
                                                       std::get<0>(book)));
        }
        endResetModel();
    }

    void ModelBook::searchVersesByText(const QString &searchVerseText) {
        if(*m_searchQueryInVerseText == searchVerseText || searchVerseText.length() < MIN_LENGTH_SEARCH_QUERY) {
            return;
        }
        m_searchQueryInVerseText = std::make_shared<QString>(searchVerseText);
        m_queryTimer->start(m_waitingTimeBeforeHitDb);
    }

    // Remove search by books, because it very heavy search
    void ModelBook::doSearchVersesByText() {
        try {
            // Reset the model
            beginResetModel();
            objectsCount = 0;
            m_objects.clear();

            // Query the database to get all books with matching verses
            auto &&data = m_db->storage->select(
                columns(rowid<Book>(), &Book::m_bookNumber, &Book::m_shortName, &Book::m_longName, &Book::m_bookColor),
                inner_join<Verse>(on(c(&Verse::m_bookNumber) == &Book::m_bookNumber)),
                where(like(&Verse::m_text, "%" + *m_searchQueryInVerseText + "%")),
                group_by(&Book::m_bookNumber),
                order_by(&Book::m_bookNumber));

            for(auto &&book: data) {
                m_objects.push_back(std::make_unique<Book>(std::get<1>(book),
                                                           std::move(std::get<2>(book)),
                                                           std::move(std::get<3>(book)),
                                                           std::move(std::get<4>(book)),
                                                           0,
                                                           std::get<0>(book)));
            }
            // End resetting the model
            endResetModel();
        } catch(const std::system_error &e) {
            // Log the error and emit an error signal
            qCritical() << "Error searching verses in ModelBook: " << e.what();
            emit error("An error occurred.");
        } catch(...) {
            throw;
        }
    }

    void ModelBook::registerMe() {
        qmlRegisterType<ModelBook>("bible.ModelBook", 1, 0, "ModelBook");
    }

    int ModelBook::getBookId(int bookNumber) {
        try {
            auto &&data = m_db->storage->select(columns(rowid<Book>()), where(c(&Book::m_bookNumber) == bookNumber));
            return data.empty() ? -1 : static_cast<int>(std::get<0>(data[0]));
        } catch(const std::system_error &e) {
            // Log the error and emit an error signal
            qCritical() << "Error searching bookNumber in ModelBook: " << e.what();
            emit error("An error occurred.");
        } catch(...) {
            throw;
        }
        return -1;
    }

    // Override
    QHash<int, QByteArray> ModelBook::roleNames() const {
        return {
            {BookNumber, "book_number"},
            {ShortName, "short_name"},
            {LongName, "long_name"},
            {BookColor, "book_color"},
            {Chapters, "chapters"},
            {NumberChapters, "number_chapters"},
            {FoundVerses, "foundVerses"},
            {BookId, "bookId"},
        };
    }

    QVariant ModelBook::data(const QModelIndex &index, int role) const {
        QVariant data{};

        if(!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &object = m_objects.at(index.row());

        switch(role) {
            case BookId:
                data = object->m_id;
                break;
            case BookNumber:
                data = object->m_bookNumber;
                break;
            case ShortName:
                data = object->m_shortName;
                break;
            case LongName:
                data = object->m_longName;
                break;
            case BookColor:
                data = object->m_bookColor;
                break;
            case Chapters:
                if(object->m_chapters == nullptr) {
                    object->m_chapters =
                        std::make_shared<ModelChapter>(object->m_bookNumber, std::move(m_db->getFileName()));
                }

#ifdef Qt6_FOUND
                data = QVariant::fromValue(object->m_chapters.get());
#else
                data = qVariantFromValue(object->m_chapters.get());
#endif
                break;
            case FoundVerses:
                if(object->m_foundVerses == nullptr) {
                    std::shared_ptr<QString> searchQueryInVerseText = m_searchQueryInVerseText;
                    object->m_foundVerses = std::make_shared<ModelVerse>(searchQueryInVerseText,
                                                                         object->m_bookNumber,
                                                                         std::move(m_db->getFileName()));
                }
#ifdef Qt6_FOUND
                data = QVariant::fromValue(object->m_foundVerses.get());
#else
                data = qVariantFromValue(object->m_foundVerses.get());
#endif
                break;
            default:
                data = QVariant();
        }

        return data;
    }
}  // namespace modules
