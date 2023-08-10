#ifndef DB_H
#define DB_H

#include <QString>
#include "record.h"
#include "binding.h"
#include "book.h"
#include "groupmodules.h"
#include "host.h"
#include "info.h"
#include "module.h"
#include "comment.h"
#include "registry.h"
#include "sqlite_orm.h"
#include "verse.h"
#include <QDebug>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace db {

    using namespace sqlite_orm;
    using namespace modules;

    inline auto translationCommentStorageFunc(const QString &name) {
        return make_storage(name.toStdString(),
                            make_table("commentaries",
                                       make_column("book_number", &Comment::m_bookNumber),
                                       make_column("chapter_number_from", &Comment::m_chapterNumberFrom),
                                       make_column("verse_number_from", &Comment::m_verseNumberFrom),
                                       make_column("chapter_number_to", &Comment::m_chapterNumberTo),
                                       make_column("verse_number_to", &Comment::m_verseNumberTo),
                                       make_column("marker", &Comment::m_marker),
                                       make_column("text", &Comment::m_text)));
    }

    inline auto translationStorageFunc(const QString &name) {

        return make_storage(name.toStdString(),
                            make_index("idx_verses_text", &Verse::m_text),
                            make_index("idx_verses_book_number", &Verse::m_bookNumber),
                            make_table("books_all",
                                       make_column("book_number", &Book::m_bookNumber),
                                       make_column("short_name", &Book::m_shortName),
                                       make_column("long_name", &Book::m_longName),
                                       make_column("book_color", &Book::m_bookColor),
                                       make_column("is_present", &Book::m_isPresent)),
                            //            make_table(
                            //                "info",
                            //                make_column("name", &Info::m_name),
                            //                make_column("value", &Info::m_value)
                            //            ),
                            make_table("verses",
                                       make_column("book_number", &Verse::m_bookNumber),
                                       make_column("chapter", &Verse::m_chapter),
                                       make_column("verse", &Verse::m_verse),
                                       make_column("text", &Verse::m_text)));
    }

    inline auto userStorage(const QString &name) {
        return make_storage(
            name.toStdString(),
            make_index("idx_id_group_modules", &Module::m_idGroupModules),
            make_index("idx_module_abbreviation", &Module::m_abbreviation),
            make_index("idx_module_name", &Module::m_name),
            make_index("idx_group_name", &GroupModules::m_name),
            make_index("idx_group_region", &GroupModules::m_region),
            make_index("idx_group_language", &GroupModules::getLanguageName),
            make_table("registries",
                       make_column("id", &Registry::m_id, primary_key()),
                       make_column("url", &Registry::m_url),
                       make_column("priority", &Registry::m_priority, default_value(0)),
                       make_column("info_url", &Registry::m_infoUrl),
                       make_column("test", &Registry::m_test, default_value(false))),
            make_table("hosts",
                       make_column("id", &Host::m_id, primary_key()),
                       make_column("alias", &Host::m_alias, default_value("")),
                       make_column("path", &Host::m_path),
                       make_column("priority", &Host::m_priority, default_value(0)),
                       make_column("weight", &Host::m_weight, default_value(0))),
            make_table("group_modules",
                       make_column("id", &GroupModules::m_id, primary_key()),
                       make_column("group_id", &GroupModules::m_groupId),
                       make_column("language", &GroupModules::m_language),
                       make_column("name", &GroupModules::m_name),
                       make_column("region", &GroupModules::m_region),
                       make_column("language_name", &GroupModules::getLanguageName, &GroupModules::setLanguageName),
                       make_column("count_modules", &GroupModules::m_countModules, default_value(0))),
            make_table("modules",
                       make_column("id", &Module::m_id, primary_key()),
                       make_column("name", &Module::m_name),
                       make_column("description", &Module::m_description, default_value("")),
                       make_column("abbreviation", &Module::m_abbreviation, default_value("")),
                       make_column("group_modules_id", &Module::m_idGroupModules, default_value(0)),
                       make_column("size", &Module::m_size, default_value(0)),
                       make_column("language_show", &Module::m_languageShow, default_value("")),
                       make_column("information", &Module::m_information, default_value("")),
                       make_column("comment", &Module::m_comment, default_value("")),
                       make_column("copyright", &Module::m_copyright, default_value("")),
                       make_column("update", &Module::m_update),
                       make_column("hidden", &Module::m_hidden, default_value(false)),
                       make_column("defaultDownload", &Module::m_defaultDownload, default_value(false)),
                       make_column("downloaded", &Module::m_downloaded, default_value(false)),
                       make_column("selected", &Module::m_selected, default_value(false)),
                       make_column("active", &Module::m_active, default_value(false))),
            make_table("record",
                       make_column("timestamp", &Record::m_timestamp),
                       make_column("book_short_name", &Record::m_bookShortName),
                       make_column("book_index", &Record::m_bookIndex),
                       make_column("chapter_index", &Record::m_chapterIndex),
                       make_column("verse_index", &Record::m_verseIndex)));
    }

    using Storage = decltype(userStorage(""));
    using TranslationStorage = decltype(translationStorageFunc(""));
    using TranslationCommentStorage = decltype(translationCommentStorageFunc(""));

    class MySingleton {
      public:
        MySingleton(const MySingleton &) = delete;
        MySingleton &operator=(const MySingleton &) = delete;

        static MySingleton &getInstance(const QString &fileName) {
            static MySingleton instance(fileName);
            return instance;
        }
        std::shared_ptr<Storage> storage;

      private:
        explicit MySingleton(const QString &fileName) {
            storage = std::make_shared<Storage>(userStorage(fileName));
            storage->sync_schema();
        }
        ~MySingleton() = default;
    };

    class TranslationStorageSingleton {
      public:
        TranslationStorageSingleton &operator=(const TranslationStorageSingleton &) = delete;
        TranslationStorageSingleton(const TranslationStorageSingleton &) = delete;

        static TranslationStorageSingleton &getInstance() {
            static TranslationStorageSingleton instance;
            return reinterpret_cast<TranslationStorageSingleton &>(instance);
        }
        std::shared_ptr<TranslationStorage> storage;

      private:
        explicit TranslationStorageSingleton() {
            storage = std::make_shared<TranslationStorage>(translationStorageFunc(""));
            storage->sync_schema();
        }
        ~TranslationStorageSingleton() = default;
    };

    class TranslationCommentStorageSingleton {
      public:
        TranslationCommentStorageSingleton &operator=(const TranslationCommentStorageSingleton &) = delete;
        TranslationCommentStorageSingleton(const TranslationCommentStorageSingleton &) = delete;

        static TranslationCommentStorageSingleton &getInstance() {
            static TranslationCommentStorageSingleton instance;
            return reinterpret_cast<TranslationCommentStorageSingleton &>(instance);
        }
        std::shared_ptr<TranslationCommentStorage> storage;

      private:
        explicit TranslationCommentStorageSingleton() {
            storage = std::make_shared<TranslationCommentStorage>(translationCommentStorageFunc(""));
            storage->sync_schema();
        }
        ~TranslationCommentStorageSingleton() = default;
    };

    template<class T, typename S = Storage>
    class Db {

      public:
        explicit Db(QString &&fileName = "");
        using vector_unique_iterator = typename std::vector<std::unique_ptr<T>>::const_iterator;
        using vector_shared_iterator = typename std::vector<std::shared_ptr<T>>::const_iterator;
        using vector_iterator = typename std::vector<T>::const_iterator;

        std::shared_ptr<S> storage;
        [[nodiscard]] QString getFileName() const;
        [[nodiscard]] QString getFullPathDbComment() const;
        void removeAll();
        int count();
        std::vector<std::unique_ptr<T>> get_all_pointer();
        void save(const vector_unique_iterator &begin, const vector_unique_iterator &end);
        void save(const vector_shared_iterator &begin, const vector_shared_iterator &end);
        void save(const MapValueIterator &begin, const MapValueIterator &end);
        void save(const vector_iterator &begin, const vector_iterator &end);

      private:
        QString m_fileName;
    };

}  // namespace db

#endif  // DB_H
