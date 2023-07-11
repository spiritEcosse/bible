//
// Created by ihor on 21.11.2022.
//

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#include <utility>
#endif

#include "verse.h"

namespace modules {

    Verse::Verse(int bookNumber, int chapter, int verse, QString &&text) :
        m_bookNumber(bookNumber), m_chapter(chapter), m_verse(verse), m_text(std::move(text)) {}

    Verse::Verse(int bookNumber, int chapter, int numberVerses, int verse) :
        m_bookNumber(bookNumber), m_chapter(chapter), m_numberVerses(numberVerses), m_verse(verse) {}

    bool Verse::operator==(const Verse &other) const {
        return m_bookNumber == other.m_bookNumber && m_chapter == other.m_chapter && m_verse == other.m_verse &&
               m_text == other.m_text && m_numberVerses == other.m_numberVerses;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug debug, const Verse &verse) {
        return debug << verse.m_bookNumber << verse.m_chapter << verse.m_verse << verse.m_text << verse.m_numberVerses;
    }
#endif

}  // namespace modules
