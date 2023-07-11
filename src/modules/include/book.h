//
// Created by ihor on 20.11.2022.
//

#ifndef BOOK_H
#define BOOK_H

#include <memory>

namespace modules {

    class ModelChapter;

    class Book {
      public:
        Book() = default;
        Book(int bookNumber,
             QString &&shortName,
             QString &&longName,
             QString &&bookColor,
             bool isPresent,
             int numberChapters);

        int m_bookNumber{};
        QString m_shortName;
        QString m_longName;
        QString m_bookColor;
        bool m_isPresent{};
        int m_numberChapters{};
        std::shared_ptr<ModelChapter> m_chapters;

        bool operator==(const Book &other) const;

#ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Book &book);
#endif
    };

    using BookShared = decltype(std::shared_ptr<Book>());
    using BookUnique = decltype(std::unique_ptr<Book>());

}  // namespace modules
#endif  // BOOK_H
