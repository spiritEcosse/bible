#ifndef DB_H
#define DB_H

#include "sqlite_orm.h"
#include "binding.h"
#include "registry.h"
#include "module.h"
#include "groupmodules.h"
#include <unordered_map>


namespace db {

    using namespace sqlite_orm;
    using namespace modules;

    inline auto userStorage(const std::string& name)
    {
        return make_storage(
                    name,
                    make_table(
                        "registries",
                        make_column("id", &Registry::m_id, primary_key()),
                        make_column("url", &Registry::m_url),
                        make_column("priority", &Registry::m_priority),
                        make_column("info_url", &Registry::m_infoUrl),
                        make_column("test", &Registry::m_test, default_value(false))
                        ),
                    make_table(
                        "group_modules",
                        make_column("id", &GroupModules::m_id, primary_key()),
                        make_column("language", &GroupModules::m_language),
                        make_column("name", &GroupModules::m_name),
                        make_column("region", &GroupModules::m_region),
                        make_column("language_name", &GroupModules::getLanguageName, &GroupModules::setLanguageName)
                        ),
                    make_table(
                        "modules",
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
                        make_column("default_download", &Module::m_defaultDownload, default_value(false))
                    ));
    }

    using Storage = decltype(userStorage(""));

    template<class T>
    class Db
    {
    private:
        Db();
        Db(const Db&);
        Db& operator=(const Db&);

    public:
        std::unique_ptr<Storage> storage;
        static std::shared_ptr<Db> getInstance();
        static std::shared_ptr<Db> m_db;
        void removeAll();
        int count();
        int64 lastInsertRowid();
        void save(
                const typename std::vector<T>::const_iterator& begin,
                const typename std::vector<T>::const_iterator& end);
    };

}

#endif // DB_H
