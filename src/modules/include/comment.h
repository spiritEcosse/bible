//
// Created by ihor on 17.12.2022.
//

#ifndef COMMENT_H
#define COMMENT_H

#include "QString"
#include <memory>

namespace modules {

    class Comment {
    public:
        Comment() = default;
        Comment(int bookNumber,
                int chapterNumberFrom,
                int verseNumberFrom,
                int chapterNumberTo,
                int verseNumberTo,
                QString &&marker,
                QString &&text);
        int m_bookNumber{};
        int m_chapterNumberFrom{};
        int m_verseNumberFrom{};
        int m_chapterNumberTo{};
        int m_verseNumberTo{};
        QString m_marker;
        QString m_text;
        bool operator==(const Comment &other) const;

#ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Comment &verse);
#endif
    };

    using CommentShared = decltype(std::shared_ptr<Comment>());
    using CommentUnique = decltype(std::unique_ptr<Comment>());
}  // namespace modules

#endif  //COMMENT_H
