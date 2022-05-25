#include "db.h"
#include "dereferenceiterator.h"
#include <QDebug>


namespace db {

    template <class T>
    Db<T>::Db()
    {
        storage = MySingleton::getInstance().storage;
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
//        storage->insert_range<T>(begin, end, [](const std::unique_ptr<T> &pointer) -> const T & {
//            return *pointer;
//        });
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
