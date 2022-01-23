#ifndef DB_H
#define DB_H

#include "sqlite_orm.h"
#include "binding.h"
#include "registry.h"
#include "module.h"
#include "host.h"
#include "groupmodules.h"
#include <unordered_map>
#include <mutex>
#include <memory>
#include <QDebug>

namespace db {

    using namespace sqlite_orm;
    using namespace modules;

    inline auto userStorage(const std::string& name)
    {
        return make_storage(
                    name,
                    make_index("idx_id_group_modules", &Module::m_idGroupModules),
                    make_index("idx_module_abbreviation", &Module::m_abbreviation),
                    make_index("idx_module_name", &Module::m_name),
                    make_index("idx_group_name", &GroupModules::m_name),
                    make_index("idx_group_region", &GroupModules::m_region),
                    make_index("idx_group_language", &GroupModules::getLanguageName),
                    make_table(
                        "registries",
                        make_column("id", &Registry::m_id, primary_key()),
                        make_column("url", &Registry::m_url),
                        make_column("priority", &Registry::m_priority, default_value(0)),
                        make_column("info_url", &Registry::m_infoUrl),
                        make_column("test", &Registry::m_test, default_value(false))
                        ),
                    make_table(
                        "hosts",
                        make_column("id", &Host::m_id, primary_key()),
                        make_column("alias", &Host::m_alias, default_value("")),
                        make_column("path", &Host::m_path),
                        make_column("priority", &Host::m_priority, default_value(0)),
                        make_column("weight", &Host::m_weight, default_value(0))
                        ),
                    make_table(
                        "group_modules",
                        make_column("id", &GroupModules::m_id, primary_key()),
                        make_column("group_id", &GroupModules::m_groupId),
                        make_column("language", &GroupModules::m_language),
                        make_column("name", &GroupModules::m_name),
                        make_column("region", &GroupModules::m_region),
                        make_column("language_name", &GroupModules::getLanguageName, &GroupModules::setLanguageName),
                        make_column("count_modules", &GroupModules::m_countModules, default_value(0))
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
                        make_column("defaultDownload", &Module::m_defaultDownload, default_value(false)),
                        make_column("downloaded", &Module::m_downloaded, default_value(false)),
                        make_column("selected", &Module::m_selected, default_value(false)),
                        make_column("active", &Module::m_active, default_value(false))
                    ));
    }

    using Storage = decltype(userStorage(""));


    class MySingleton{
    public:
        static MySingleton& getInstance(){
            static MySingleton instance;
            return instance;
        }
        std::shared_ptr<Storage> storage;
    private:
        MySingleton() {
            storage.reset(new Storage(userStorage("user.sqlite")));
            storage->sync_schema();
        }
        ~MySingleton()= default;
        MySingleton(const MySingleton&)= delete;
        MySingleton& operator=(const MySingleton&)= delete;
    };

    template<class T>
    class Db
    {

    public:
        Db();
        using vector_unique_iterator = typename std::vector<std::unique_ptr<T>>::const_iterator;
        using vector_shared_iterator = typename std::vector<std::shared_ptr<T>>::const_iterator;
        using vector_iterator = typename std::vector<T>::const_iterator;

        std::shared_ptr<Storage> storage;
        void removeAll();
        int count();
        int64 lastInsertRowid();
        void save(
                const vector_unique_iterator& begin,
                const vector_unique_iterator& end);
        void save(
                const vector_shared_iterator& begin,
                const vector_shared_iterator& end);
        void save(
                const MapValueIterator& begin,
                const MapValueIterator& end);
        void save(
                const vector_iterator& begin,
                const vector_iterator& end);
    };

}

#endif // DB_H
