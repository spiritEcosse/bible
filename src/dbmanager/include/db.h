#ifndef DB_H
#define DB_H

#include "sqlite_orm.h"
#include "binding.h"
#include "registry.h"


namespace db {

    using namespace sqlite_orm;

    inline auto userStorage(const std::string& name)
    {
        return make_storage(
                    name,
                    make_table(
                        "registries",
                        make_column("id", &Registry::m_id, primary_key(), autoincrement()),
                        make_column("url", &Registry::m_url),
                        make_column("priority", &Registry::m_priority),
                        make_column("info_url", &Registry::m_infoUrl)));
    }

    template<class O, class T, class ...Op>
    using Column = internal::column_t<O, T, const T& (O::*)() const, void (O::*)(T), Op...>;
    using Storage = internal::storage_t<internal::table_t<Registry,
    Column<Registry, decltype(Registry::m_id), constraints::primary_key_t<>, constraints::autoincrement_t>,
    Column<Registry, decltype(Registry::m_url)>,
    Column<Registry, decltype(Registry::m_priority)>,
    Column<Registry, decltype(Registry::m_infoUrl)>>>;

    static_assert(std::is_same<Storage, decltype(userStorage(""))>::value, "");

    class Db
    {
    public:
        static std::shared_ptr<Db> getInstance();
        std::unique_ptr<Storage> storage;
        static std::shared_ptr<Db> m_db;
    private:
        Db();
        Db(const Db&);
        Db& operator=(const Db&);
    };

}

#endif // DB_H
