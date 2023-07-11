//
// Created by ihor on 20.11.2022.
//

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#include <utility>
#endif

#include "book.h"

namespace modules {

    Book::Book(int bookNumber,
               QString &&shortName,
               QString &&longName,
               QString &&bookColor,
               bool isPresent,
               int numberChapters) :
        m_bookNumber(bookNumber),
        m_shortName(std::move(shortName)), m_longName(std::move(longName)), m_bookColor(std::move(bookColor)),
        m_isPresent(isPresent), m_numberChapters(numberChapters) {}

    bool Book::operator==(const Book &other) const {
        return m_bookNumber == other.m_bookNumber && m_shortName == other.m_shortName &&
               m_longName == other.m_longName && m_bookColor == other.m_bookColor && m_isPresent == other.m_isPresent &&
               m_numberChapters == other.m_numberChapters;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug debug, const Book &book) {
        return debug << book.m_bookNumber << book.m_shortName << book.m_longName << book.m_bookColor << book.m_isPresent
                     << book.m_numberChapters;
    }
#endif

}  // namespace modules