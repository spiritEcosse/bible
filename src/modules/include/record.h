//
// Created by ihor on 20.12.2022.
//

#ifndef RECORD_H
#define RECORD_H

#include <QDate>
#include <QString>
#include <memory>

namespace modules {
    class Record {
    public:
        Record() = default;
        Record(int bookNumber,
               int bookIndex,
               int chapterIndex,
               int verseIndex,
               QDateTime &&timestamp = QDateTime::currentDateTime(),
               QString &&bookShortName = "");
        int m_bookNumber;
        int m_bookIndex{};
        int m_chapterIndex{};
        int m_verseIndex{};
        QDateTime m_timestamp;
        QString m_bookShortName;

        bool operator==(const Record &other) const;

#ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Record &verse);
#endif
    };

    using RecordShared = decltype(std::shared_ptr<Record>());
    using RecordUnique = decltype(std::unique_ptr<Record>());
}  // namespace modules
#endif  //RECORD_H
