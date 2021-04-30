#include "db.h"
#include <QDebug>
#include "dereferenceiterator.h"

namespace db {

    template <class T>
    std::unique_ptr<Db<T>> Db<T>::m_db = nullptr;

    template <class T>
    Db<T>::Db()
    {
        storage.reset(new Storage(userStorage("user.sqlite")));
        storage->sync_schema();
    }

    template <class T>
    std::unique_ptr<Db<T>> Db<T>::getInstance()
    {
        if (m_db == nullptr)
        {
            m_db.reset(new Db<T> {});
        }
        return std::move(m_db);
    }

    template <class T>
    void Db<T>::removeAll()
    {
        storage->remove_all<T>();
    }

    template<class T>
    int Db<T>::count()
    {
        return storage->count<T>();
    }

    template<class T>
    int64 Db<T>::lastInsertRowid()
    {
        return storage->last_insert_rowid();
    }

    template<class T>
    void Db<T>::save(const vector_unique_iterator& begin, const vector_unique_iterator& end)
    {
        storage->insert_range(dereference_iterator(begin), dereference_iterator(end));
    }

    template<class T>
    void Db<T>::save(const vector_shared_iterator& begin, const vector_shared_iterator& end)
    {
        storage->insert_range(dereference_iterator(begin), dereference_iterator(end));
    }

    template<class T>
    void Db<T>::save(const vector_iterator& begin, const vector_iterator& end)
    {
        storage->insert_range(begin, end);
    }

    template<class T>
    void Db<T>::save(const MapValueIterator &begin, const MapValueIterator &end)
    {
        storage->insert_range(begin, end);
    }

    template class Db<Registry>;
    template class Db<Module>;
    template class Db<GroupModules>;
    template class Db<Host>;
}
