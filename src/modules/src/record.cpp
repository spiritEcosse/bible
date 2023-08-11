//
// Created by ihor on 20.12.2022.
//

#include "record.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#include <utility>
#endif

namespace modules {
    Record::Record(QString &&bookShortName, int bookIndex, int chapterIndex, int verseIndex, QDateTime &&timestamp) :
        m_timestamp(std::move(timestamp)), m_bookShortName(std::move(bookShortName)), m_bookIndex(bookIndex),
        m_chapterIndex(chapterIndex), m_verseIndex(verseIndex) {}

    bool Record::operator==(const Record &other) const {
        return m_timestamp == other.m_timestamp && m_bookShortName == other.m_bookShortName &&
               m_bookIndex == other.m_bookIndex && m_chapterIndex == other.m_chapterIndex &&
               m_verseIndex == other.m_verseIndex;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug debug, const Record &object) {
        return std::move(debug) << object.m_timestamp << object.m_bookShortName << object.m_bookIndex
                                << object.m_chapterIndex << object.m_verseIndex;
    }
#endif

}  // namespace modules