#include "basetest.h"
#include "modelmodule.h"
#include "modelgroupmodules.h"
#include "modelmoduledownload.h"
#include "modelregistry.h"
#include "modelhost.h"
#include "dereferenceiterator.h"


namespace tests {

    template <class T, class O>
    BaseTest<T, O>::BaseTest() {}

    template <class T, class O>
    BaseTest<T, O>::~BaseTest() {}

    template <class T, class O>
    void BaseTest<T, O>::initTestCase()
    {
        // Will be called before the first test function is executed.
        dir.mkdir(pathFiles);
        dir.setCurrent(pathFiles);
        dir.mkdir(dirDownload);
        m_db.reset(new db::Db<T>());
        cleanTable();
    }

    template <class T, class O>
    void BaseTest<T, O>::cleanupTestCase()
    {
        dir.rmdir(dirDownload);
    }

    template <class T, class O>
    std::vector<std::shared_ptr<T>>
    BaseTest<T, O>::helperSave(std::vector<std::shared_ptr<T>>&& entries)
    {
        const auto &objects = entries.size() == 0 ? helperGetObjects() : std::move(entries);
        m_db->save(objects.begin(), objects.end());
        return std::move(objects);
    }

    template <class T, class O>
    void BaseTest<T, O>::cleanTable()
    {
        m_db->removeAll();
    }

    template <class T, class O>
    std::vector<std::shared_ptr<T>> BaseTest<T, O>::helperGetObjects() const
    {
        return std::vector<std::shared_ptr<T>>{};
    }

    template<class T, class O>
    std::vector<std::unique_ptr<T>> BaseTest<T, O>::helperGetObjectsUnique() const
    {
        return std::vector<std::unique_ptr<T>>{};
    }

    //tests

    template class BaseTest<modules::Module, modules::ModelModule>;
    template class BaseTest<modules::GroupModules, modules::ModelGroupModules>;
    template class BaseTest<modules::Registry, modules::ModelRegistry>;
    template class BaseTest<modules::Host, modules::ModelHost>;
}
