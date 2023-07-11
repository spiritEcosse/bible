//
// Created by ihor on 21.11.2022.
//

#include "modelbook.h"
#include "modelchapter.h"
#include <memory>

namespace modules {

    ModelBook::ModelBook(QString &&fileName, QObject *parent) :
        ListModel<Book, db::TranslationStorage>(std::move(fileName), parent) {
        updateObjects();
    }

    void ModelBook::updateObjects() {
        beginResetModel();
        objectsCount = 0;
        auto &&data = m_db->storage->select(columns(&Book::m_bookNumber,
                                                    &Book::m_shortName,
                                                    &Book::m_longName,
                                                    &Book::m_bookColor,
                                                    &Book::m_isPresent,
                                                    sqlite_orm::max(&Verse::m_chapter)),
                                            inner_join<Verse>(on(c(&Book::m_bookNumber) == &Verse::m_bookNumber)),
                                            group_by(&Book::m_bookNumber),
                                            order_by(&Book::m_bookNumber));

        for(auto &&book: data) {
            m_objects.push_back(std::make_unique<Book>(std::get<0>(book),
                                                       std::move(std::get<1>(book)),
                                                       std::move(std::get<2>(book)),
                                                       std::move(std::get<3>(book)),
                                                       std::get<4>(book),
                                                       *std::get<5>(book).get()));
        }
        endResetModel();
    }

    // Override
    QHash<int, QByteArray> ModelBook::roleNames() const {
        return {
            {BookNumber, "book_number"},
            {ShortName, "short_name"},
            {LongName, "long_name"},
            {BookColor, "book_color"},
            {IsPresent, "is_present"},
            {Chapters, "chapters"},
            {NumberChapters, "number_chapters"},
        };
    }

    QVariant ModelBook::data(const QModelIndex &index, int role) const {
        QVariant data{};

        if(!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &object = m_objects.at(index.row());

        switch(role) {
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
            case IsPresent:
                data = object->m_isPresent;
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
            default:
                data = QVariant();
        }

        return data;
    }
}  // namespace modules
