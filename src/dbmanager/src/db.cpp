#include "db.h"
#include <QDebug>

namespace db {

    template <class T>
    std::shared_ptr<Db<T>> Db<T>::m_db = nullptr;

    template <class T>
    Db<T>::Db()
    {
        storage.reset(new Storage(userStorage("user.sqlite")));
        storage->sync_schema();
    }

    template <class T>
    std::shared_ptr<Db<T>> Db<T>::getInstance()
    {
        m_db.reset(new Db<T> {});
        return m_db;
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
    void Db<T>::save(
            const typename std::vector<T>::const_iterator& begin,
            const typename std::vector<T>::const_iterator& end)
    {
        storage->insert_range(begin, end);
    }

    template class Db<Registry>;
    template class Db<Module>;
    template class Db<GroupModules>;
}
