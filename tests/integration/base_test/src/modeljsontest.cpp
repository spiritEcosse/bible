#include "modelregistry.h"
#include "modelhost.h"
#include "modelgroupmodules.h"
#include "modelmodule.h"
#include "dereferenceiterator.h"

#include "modeljsontest.h"

Q_DECLARE_METATYPE(modules::RegistryShared)
Q_DECLARE_METATYPE(std::vector<modules::RegistryShared>)
Q_DECLARE_METATYPE(modules::ModuleShared)
Q_DECLARE_METATYPE(std::vector<modules::Registry>)
Q_DECLARE_METATYPE(std::vector<modules::GroupModulesShared>)
Q_DECLARE_METATYPE(std::vector<modules::HostShared>)

namespace tests {

    template <class T, class O>
    ModelJsonTest<T, O>::ModelJsonTest() {}

    template <class T, class O>
    ModelJsonTest<T, O>::~ModelJsonTest() {}

    template<class T, class O>
    void ModelJsonTest<T, O>::initTestCase()
    {
        BaseTest<T, O>::initTestCase();
    }

    template<class T, class O>
    void ModelJsonTest<T, O>::cleanupTestCase()
    {
        BaseTest<T, O>::cleanupTestCase();
    }

    template <class T, class O>
    QJsonDocument ModelJsonTest<T, O>::helperGetDocument()
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
    QJsonDocument ModelJsonTest<T, O>::helperGetInvalidDocument() const
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

    // tests
    template <class T, class O>
    void ModelJsonTest<T, O>::update()
    {
        O model;
        QSignalSpy spyLast(&model, &O::updateDone);

        const auto &objects = helperGetObjectsUnique();
        model.updateUnique(objects);

        QCOMPARE(spyLast.count(), 1);
        int count = BaseTest<T, O>::m_db->count();
        QCOMPARE(count, static_cast<int>(objects.size()));
        const auto &m_objects = BaseTest<T, O>::m_db->storage->template get_all<T>();
        QCOMPARE(std::equal(m_objects.begin(),
                    m_objects.end(),
                   dereference_iterator(objects.begin())
                   ), true);
    }

    template <class T, class O>
    void ModelJsonTest<T, O>::transform_data()
    {
        QTest::addColumn<QJsonDocument>("document");
        QTest::addColumn<std::vector<ModelShared>>("objects");
        QTest::addColumn<bool>("hit");

        QTest::newRow("valid data") << helperGetDocument() << helperGetObjects() << true;
        QTest::newRow("not valid data") << helperGetInvalidDocument() << std::vector<ModelShared>() << false;
    }

    template <class T, class O>
    void ModelJsonTest<T, O>::transform()
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

    template class ModelJsonTest<modules::Module, modules::ModelModule>;
    template class ModelJsonTest<modules::GroupModules, modules::ModelGroupModules>;
    template class ModelJsonTest<modules::Registry, modules::ModelRegistry>;
    template class ModelJsonTest<modules::Host, modules::ModelHost>;

}
