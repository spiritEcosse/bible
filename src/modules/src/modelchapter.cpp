//
// Created by ihor on 06.12.2022.
//

#include "modelchapter.h"
#include "modelverse.h"

namespace modules {

    ModelChapter::ModelChapter(QString &&fileName, QObject *parent) :
        ListModel<Verse, db::TranslationStorage>(std::move(fileName), parent) {}

    ModelChapter::ModelChapter(int bookNumber, QString &&fileName, QObject *parent) :
        ListModel<Verse, db::TranslationStorage>(std::move(fileName), parent), m_bookNumber(bookNumber) {
        updateObjects();
    }

    void ModelChapter::updateObjects() {
        beginResetModel();
        objectsCount = 0;
        auto &&data = m_db->storage->select(
            columns(&Verse::m_bookNumber, &Verse::m_chapter, sqlite_orm::max(&Verse::m_verse), &Verse::m_verse),
            where(c(&Verse::m_bookNumber) == m_bookNumber),
            group_by(&Verse::m_chapter),
            order_by(&Verse::m_chapter));

        for(auto &&verse: data) {
            m_objects.push_back(std::make_unique<Verse>(std::get<0>(verse),
                                                        std::get<1>(verse),
                                                        *std::get<2>(verse).get(),
                                                        std::get<3>(verse)));
        }
        endResetModel();
    }

    // Override
    QHash<int, QByteArray> ModelChapter::roleNames() const {
        return {
            {BookNumber, "book_number"},
            {Chapter, "chapter"},
            {NumberVerses, "number_verses"},
            {Verses, "verses"},
        };
    }

    QVariant ModelChapter::data(const QModelIndex &index, int role) const {
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
            case NumberVerses:
                data = object->m_numberVerses;
                break;
            case Verses:
                if(object->m_verses == nullptr) {
                    object->m_verses = std::make_shared<ModelVerse>(object->m_bookNumber,
                                                                    object->m_chapter,
                                                                    std::move(m_db->getFileName()));
                }

#ifdef Qt6_FOUND
                data = QVariant::fromValue(object->m_verses.get());
#else
                data = qVariantFromValue(object->m_verses.get());
#endif
                break;
            default:
                data = QVariant();
        }

        return data;
    }
}  // namespace modules
