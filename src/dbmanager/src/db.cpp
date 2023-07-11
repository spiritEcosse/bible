#include "db.h"
#include "dereferenceiterator.h"
#include <QFileInfo>

namespace db {

    template<class T, typename S>
    Db<T, S>::Db(QString &&fileName) : m_fileName(std::move(fileName)) {
        if constexpr(std::is_same_v<Storage, S>) {
            if(m_fileName != ":memory:") {
                m_fileName = "user.sqlite";
            }
            storage = MySingleton::getInstance(m_fileName).storage;
        } else if constexpr(std::is_same_v<TranslationStorage, S>) {
            if(m_fileName == ":memory:" || m_fileName.isEmpty()) {
                storage = TranslationStorageSingleton::getInstance().storage;
            } else {
                storage.reset(new TranslationStorage(translationStorageFunc(m_fileName)));
            }
        } else if constexpr(std::is_same_v<TranslationCommentStorage, S>) {
            if(m_fileName == ":memory:" || m_fileName.isEmpty()) {
                storage = TranslationCommentStorageSingleton::getInstance().storage;
            } else {
                storage.reset(new TranslationCommentStorage(translationCommentStorageFunc(m_fileName)));
            }
        }
    }

    template<class T, typename S>
    QString Db<T, S>::getFileName() const {
        return m_fileName;
    }

    template<class T, typename S>
    QString Db<T, S>::getFullPathDbComment() const {
        QString fullPathFile = m_fileName.split(".SQLite3").first() + "/.commentaries.SQLite3";
        QFileInfo fileInfo(fullPathFile);

        if(!(fileInfo.exists() && fileInfo.isFile())) {
            qCritical("File '%s' does not exist!", qUtf8Printable(fullPathFile));
        }
        return fullPathFile;
    }

    template<class T, typename S>
    void Db<T, S>::removeAll() {
        storage->template remove_all<T>();
    }

    template<class T, typename S>
    int Db<T, S>::count() {
        return storage->template count<T>();
    }

    template<class T, typename S>
    std::vector<std::unique_ptr<T>> Db<T, S>::get_all_pointer() {
        return storage->template get_all_pointer<T>();
    }

    template<class T, typename S>
    void Db<T, S>::save(const vector_unique_iterator &begin, const vector_unique_iterator &end) {
        //        storage->insert_range<T>(begin, end, [](const std::unique_ptr<T>
        //        &pointer) -> const T & {
        //            return *pointer;
        //        });
        storage->insert_range(dereference_iterator(begin), dereference_iterator(end));
    }

    template<class T, typename S>
    void Db<T, S>::save(const vector_shared_iterator &begin, const vector_shared_iterator &end) {
        storage->insert_range(dereference_iterator(begin), dereference_iterator(end));
    }

    template<class T, typename S>
    void Db<T, S>::save(const vector_iterator &begin, const vector_iterator &end) {
        storage->insert_range(begin, end);
    }

    template<class T, typename S>
    void Db<T, S>::save(const MapValueIterator &begin, const MapValueIterator &end) {
        if constexpr(std::is_same_v<Storage, S>) {
            storage->insert_range(begin, end);
        }
    }

    template class Db<Registry>;
    template class Db<Module>;
    template class Db<GroupModules>;
    template class Db<Host>;
    template class Db<Record>;
    template class Db<Book, TranslationStorage>;
    template class Db<Verse, TranslationStorage>;
    template class Db<Comment, TranslationCommentStorage>;
}  // namespace db
