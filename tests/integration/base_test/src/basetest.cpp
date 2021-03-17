#include "basetest.h"
#include "modelmodule.h"
#include "modelgroupmodules.h"
#include "modelregistry.h"

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
//        cleanTable();
    }

    template <class T, class O>
    void BaseTest<T, O>::cleanupTestCase()
    {
        dir.rmdir(dirDownload);
    }

    template <class T, class O>
    std::vector<T> BaseTest<T, O>::helperSave() const
    {
        const std::vector<T>& entries = helperGetObjects();
        m_db->save(entries.begin(), entries.end());
        return entries;
    }

    template <class T, class O>
    void BaseTest<T, O>::cleanTable()
    {
        m_db->removeAll();
    }

    template <class T, class O>
    std::vector<T> BaseTest<T, O>::helperGetObjects() const
    {
        return std::vector<T>{};
    }

    // tests
    template <class T, class O>
    void BaseTest<T, O>::update()
    {
        O model;
        QSignalSpy spyLast(&model, &O::updateDone);

        const std::vector<T>& objects = helperGetObjects();
        model.update(objects);

        QCOMPARE(spyLast.count(), 1);
        QCOMPARE(m_db->count(), static_cast<int>(vectorSize));
    }

    template class BaseTest<modules::Module, modules::ModelModule>;
    template class BaseTest<modules::GroupModules, modules::ModelGroupModules>;
    template class BaseTest<modules::Registry, modules::ModelRegistry>;
}
