//
// Created by ihor on 20.11.2022.
//

#ifndef VERSE_H
#define VERSE_H

#include <memory>

namespace modules {

    class ModelVerse;
    class ModelComment;

    class Verse {
      public:
        Verse() = default;
        explicit Verse(int bookNumber, int chapter, int numberVerses, int verse);
        Verse(int bookNumber, int chapter, int verse, QString &&text);
        int m_bookNumber{};
        int m_chapter{};
        int m_verse{};
        QString m_text;
        int m_numberVerses{};
        std::shared_ptr<ModelVerse> m_verses;
        std::shared_ptr<ModelComment> m_comments;

        bool operator==(const Verse &other) const;

#ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Verse &verse);
#endif
    };

    using VerseShared = decltype(std::shared_ptr<Verse>());
    using VerseUnique = decltype(std::unique_ptr<Verse>());

}  // namespace modules

#endif  // VERSE_H
