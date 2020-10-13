#ifndef DB_H
#define DB_H

#include "sqlite_orm.h"
#include "binding.h"
#include "registry.h"
#include "module.h"


namespace db {

    using namespace sqlite_orm;
    using namespace modules;

    inline auto userStorage(const std::string& name)
    {
        //    QString* qStringSql = new QString(
        //                "CREATE TABLE IF NOT EXISTS '%1' ("
        //                "   'id'                INTEGER PRIMARY KEY AUTOINCREMENT, "
        //                "   'name'              CHAR(200) NOT NULL, "
        //                "   'description'       TEXT, "
        //                "   'abbreviation'      CHAR(50), "
        //                "   'information'       TEXT, "
        //                "   'language'          CHAR(50), "
        //                "   'language_show'     CHAR(50), "
        //                "   'update'            TEXT, "
        //                "   'urls'              TEXT, "
        //                "   'comment'           TEXT, "
        //                "   'size'              NUMERIC NOT NULL, "
        //                "   'region'            TEXT, "
        //                "   'default_download'  NUMERIC DEFAULT 0, "
        //                "   'hidden'            NUMERIC DEFAULT 0, "
        //                "   'copyright'         TEXT, "
        //                "   '%2_id'             NUMERIC NOT NULL, "
        //                "FOREIGN KEY ('%2_id')  REFERENCES %2(id)"
        //                ")"
        //                );

        return make_storage(
                    name,
                    make_table(
                        "registries",
                        make_column("id", &Registry::m_id, primary_key(), autoincrement()),
                        make_column("url", &Registry::m_url),
                        make_column("priority", &Registry::m_priority),
                        make_column("info_url", &Registry::m_infoUrl),
                        make_column("test", &Registry::m_test, default_value(false))),
                    make_table(
                        "modules",
                        make_column("id", &Module::m_id, primary_key(), autoincrement()),
//                        make_column("name", &Module::m_name),
//                        make_column("description", &Module::m_description),
//                        make_column("abbreviation", &Module::m_abbreviation),
//                        make_column("size", &Module::m_size),
//                        make_column("information", &Module::m_information),
//                        make_column("comment", &Module::m_comment),
//                        make_column("copyright", &Module::m_copyright),
                        make_column("update", &Module::m_update)
//                        make_column("hidden", &Module::m_hidden),
//                        make_column("default_download", &Module::m_defaultDownload),
//                        make_column("language_show", &Module::m_languageShow)
                        ));
    }

    template<class O, class T, class ...Op>
    using Column = internal::column_t<O, T, const T& (O::*)() const, void (O::*)(T), Op...>;
    using Storage = internal::storage_t<
    internal::table_t<Registry,
    Column<Registry, decltype(Registry::m_id), constraints::primary_key_t<>, constraints::autoincrement_t>,
    Column<Registry, decltype(Registry::m_url)>,
    Column<Registry, decltype(Registry::m_priority)>,
    Column<Registry, decltype(Registry::m_infoUrl)>,
    Column<Registry, decltype(Registry::m_test), constraints::default_t<bool>>>,
    internal::table_t<Module,
        Column<Module, decltype(Module::m_id), constraints::primary_key_t<>, constraints::autoincrement_t>,
//        Column<Module, decltype(Module::m_name)>,
//        Column<Module, decltype(Module::m_description)>,
//        Column<Module, decltype(Module::m_abbreviation)>,
//        Column<Module, decltype(Module::m_size)>,
//        Column<Module, decltype(Module::m_information)>,
//        Column<Module, decltype(Module::m_comment)>,
//        Column<Module, decltype(Module::m_copyright)>,
        Column<Module, decltype(Module::m_update)>
//        Column<Module, decltype(Module::m_hidden)>,
//        Column<Module, decltype(Module::m_defaultDownload)>,
//        Column<Module, decltype(Module::m_languageShow)>
    >>;

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
