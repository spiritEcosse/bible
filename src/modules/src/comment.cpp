//
// Created by ihor on 17.12.2022.
//

#include "comment.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#include <utility>
#endif

namespace modules {

    Comment::Comment(int bookNumber,
                     int chapterNumberFrom,
                     int verseNumberFrom,
                     int chapterNumberTo,
                     int verseNumberTo,
                     QString &&marker,
                     QString &&text) :
        m_bookNumber(bookNumber),
        m_chapterNumberFrom(chapterNumberFrom), m_verseNumberFrom(verseNumberFrom), m_chapterNumberTo(chapterNumberTo),
        m_verseNumberTo(verseNumberTo), m_marker(std::move(marker)), m_text(std::move(text)) {}

    bool Comment::operator==(const Comment &other) const {
        return m_bookNumber == other.m_bookNumber && m_chapterNumberFrom == other.m_chapterNumberFrom &&
               m_verseNumberFrom == other.m_verseNumberFrom && m_chapterNumberTo == other.m_chapterNumberTo &&
               m_verseNumberTo == other.m_verseNumberTo && m_marker == other.m_marker && m_text == other.m_text;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug debug, const Comment &object) {
        return std::move(debug) << object.m_bookNumber << object.m_chapterNumberFrom << object.m_verseNumberFrom
                                << object.m_chapterNumberTo << object.m_verseNumberTo << object.m_marker
                                << object.m_text;
    }
#endif

}  // namespace modules