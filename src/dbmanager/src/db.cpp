#include "db.h"

namespace db {

    std::shared_ptr<Db> Db::m_db = nullptr;

    Db::Db()
    {
        storage.reset(new Storage(userStorage("")));
        storage->sync_schema();
    }

    std::shared_ptr<Db> Db::getInstance()
    {
        if (!m_db) {
            m_db.reset(new Db {});
        }
        return m_db;
    }

}
