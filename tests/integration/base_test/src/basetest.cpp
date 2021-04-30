#include "basetest.h"
#include "modelmodule.h"
#include "modelgroupmodules.h"
#include "modelregistry.h"
#include "modelhost.h"
#include "dereferenceiterator.h"

Q_DECLARE_METATYPE(std::vector<modules::RegistryShared>)
Q_DECLARE_METATYPE(modules::ModuleShared)
Q_DECLARE_METATYPE(std::vector<modules::Registry>)
Q_DECLARE_METATYPE(std::vector<modules::GroupModulesShared>)
Q_DECLARE_METATYPE(std::vector<modules::HostShared>)

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
        m_db = db::Db<T>::getInstance();
        cleanTable();
    }

    template <class T, class O>
    void BaseTest<T, O>::cleanupTestCase()
    {
        dir.rmdir(dirDownload);
    }

    template <class T, class O>
    QJsonDocument BaseTest<T, O>::helperGetDocument()
    {
        QJsonArray array;
        array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};
        array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};
        array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};

        QJsonArray arrayHosts;
        arrayHosts << QJsonObject {{"alias", "alias"}, {"priority", 1}, {"weight", 2}, {"path", "link11"}};
        arrayHosts << QJsonObject {{"alias", "alias"}, {"priority", 1}, {"weight", 2}, {"path", "link11"}};
        arrayHosts << QJsonObject {{"alias", "alias"}, {"priority", 1}, {"weight", 2}, {"path", "link11"}};

        return QJsonDocument {
            QJsonObject {
                { "registries",  array },
                { "downloads", {{"key", "val"}} },
                { "version", 1 },
                { "hosts", arrayHosts },
            }
        };
    }

    template <class T, class O>
    QJsonDocument BaseTest<T, O>::helperGetInvalidDocument() const
    {
        QJsonArray array;

        array << QJsonObject {{"url", "link1"}, {"priority", 1}};
        array << QJsonObject {{"priority", 2}, {"info_ufrl", "link22"}};
        array << QJsonObject {{"url", "link3"}, {"priority", 3}, {"info_url", "link33"}};

        return QJsonDocument {
            QJsonObject {
                { "registries",  array },
                { "hosts",  QJsonArray { QJsonObject {{"alias", "alias"}, {"priority", 1}}} },
            }
        };
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

    // tests
    template <class T, class O>
    void BaseTest<T, O>::update()
    {
        O model;
        QSignalSpy spyLast(&model, &O::updateDone);

        const auto &objects = helperGetObjectsUnique();
        model.updateUnique(objects);

        QCOMPARE(spyLast.count(), 1);
        QCOMPARE(m_db->count(), static_cast<int>(objects.size()));
        const auto &m_objects = m_db->storage->template get_all<T>();
        QCOMPARE(std::equal(m_objects.begin(),
                    m_objects.end(),
                   dereference_iterator(objects.begin())
                   ), true);
    }

    template <class T, class O>
    void BaseTest<T, O>::transform_data()
    {
        QTest::addColumn<QJsonDocument>("document");
        QTest::addColumn<std::vector<ModelShared>>("objects");
        QTest::addColumn<bool>("hit");

        QTest::newRow("valid data") << helperGetDocument() << helperGetObjects() << true;
        QTest::newRow("not valid data") << helperGetInvalidDocument() << std::vector<ModelShared>() << false;
    }

    template <class T, class O>
    void BaseTest<T, O>::transform()
    {
        qRegisterMetaType<std::vector<T>>("std::vector<T>");

        QFETCH(QJsonDocument, document);
        QFETCH(std::vector<ModelShared>, objects);
        QFETCH(bool, hit);

        O model;
        QSignalSpy spy(&model, &O::transformSuccess);
        model.transform(document);

        QCOMPARE(spy.count(), int(hit));

        if (hit) {
            QCOMPARE(model.m_objectsFromJson.size(), objects.size());
            QCOMPARE(std::equal(model.m_objectsFromJson.begin(),
                       model.m_objectsFromJson.end(),
                       dereference_iterator(objects.begin())
                       ), true);
        }
    }

    template class BaseTest<modules::Module, modules::ModelModule>;
    template class BaseTest<modules::GroupModules, modules::ModelGroupModules>;
    template class BaseTest<modules::Registry, modules::ModelRegistry>;
    template class BaseTest<modules::Host, modules::ModelHost>;
}
