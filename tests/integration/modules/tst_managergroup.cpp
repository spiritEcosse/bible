#include <QtTest>
#include <JlCompress.h>
#include "managergroup.h"
#include "groupmodules.h"
#include "managerregistry.h"
#include "modelregistry.h"
#include "module.h"
#include "dereferenceiterator.h"
#include "modelgroupmodules.h"
#include "modeljsontest.h"

Q_DECLARE_METATYPE(std::vector<modules::Module>)
Q_DECLARE_METATYPE(std::vector<modules::GroupModulesShared>)

namespace modules {

    namespace tests {

        class tst_ManagerGroup : public ::tests::ModelJsonTest<Registry, ModelRegistry>
        {
            Q_OBJECT

        public:
            tst_ManagerGroup();
            ~tst_ManagerGroup();

        private:
            QJsonDocument helperGetInvalidDocument() const;
            std::vector<RegistryUnique> helperGetObjectsUnique() const;
            std::vector<RegistryShared> helperGetObjects() const;

        private slots:
            void initTestCase();
            void cleanupTestCase();
            void makeCollections_data();
            void makeCollections();
        };

        tst_ManagerGroup::tst_ManagerGroup() {}

        tst_ManagerGroup::~tst_ManagerGroup() {}

        void tst_ManagerGroup::initTestCase()
        {
            // Will be called before the first test function is executed.
            dir.mkdir(pathFiles);
            dir.setCurrent(pathFiles);
            dir.mkdir(dirDownload);
        }

        void tst_ManagerGroup::cleanupTestCase()
        {
            dir.rmdir(dirDownload);
        }

        // helpers
        QJsonDocument tst_ManagerGroup::helperGetInvalidDocument() const
        {
            QJsonArray array;

            array << QJsonObject {{"des", "des"}, {"abb", "abb"}};

            return QJsonDocument {
                QJsonObject {
                    { "downloads",  array }
                }
            };
        }

        std::vector<RegistryUnique> tst_ManagerGroup::helperGetObjectsUnique() const
        {
            return std::vector<RegistryUnique>{};
        }

        std::vector<RegistryShared> tst_ManagerGroup::helperGetObjects() const
        {
            std::vector<RegistryShared> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_shared<Registry>("bGluazE=", "bGluazEx", 1));
            }
            return objects;
        }

        // tests
        void tst_ManagerGroup::makeCollections_data()
        {
            QTest::addColumn<QJsonDocument>("document");
            QTest::addColumn<std::vector<Module>>("modules");
            QTest::addColumn<std::vector<GroupModulesShared>>("groupModules");
            QTest::addColumn<bool>("hit");

            std::vector<Module> modules;
            std::vector<GroupModulesShared> groupModules;
            QJsonArray array;

            int m_idGroupModules = 1;
            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "en"}};
            modules.push_back(Module("name", "des", "abbr", m_idGroupModules));
            groupModules.push_back(std::make_shared<GroupModules>("en", "Translations", "", m_idGroupModules));
            QTest::newRow("case: count GroupModules is 1, count Module is 1")
                    << QJsonDocument { QJsonObject {{"downloads", array }}} << modules << groupModules << true;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "en"}};
            modules.push_back(Module("name", "des", "abbr", m_idGroupModules));
            QTest::newRow("case: count GroupModules is 1, count Module is 2")
                    << QJsonDocument { QJsonObject {{"downloads", array }}} << modules << groupModules << true;

            m_idGroupModules = 2;
            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "en"},{"reg", "region"}};
            modules.push_back(Module("name", "des", "abbr", m_idGroupModules));
            groupModules.insert(groupModules.begin(),
                                std::make_shared<GroupModules>("en", "Translations", "region", m_idGroupModules));
            QTest::newRow("case: count GroupModules is 2, count Module is 3")
                    << QJsonDocument { QJsonObject {{"downloads", array }}} << modules << groupModules << true;

            m_idGroupModules = 3;
            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", ""}};
            modules.push_back(Module("name", "des", "abbr", m_idGroupModules));
            groupModules.insert(groupModules.begin(),
                                std::make_shared<GroupModules>("av", "Translations", "", m_idGroupModules));
            QTest::newRow("case: count GroupModules is 3, count Module is 4")
                    << QJsonDocument { QJsonObject {{"downloads", array }}} << modules << groupModules << true;

            m_idGroupModules = 4;
            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", "region"}};
            modules.push_back(Module("name", "des", "abbr", m_idGroupModules));
            groupModules.insert(groupModules.begin(),
                                std::make_shared<GroupModules>("av", "Translations", "region", m_idGroupModules));
            QTest::newRow("case: count GroupModules is 4, count Module is 5")
                    << QJsonDocument { { QJsonObject {{"downloads", array }}}} << modules << groupModules << true;

            m_idGroupModules = 3;
            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", ""}};
            modules.push_back(Module("name", "des", "abbr", m_idGroupModules));
            QTest::newRow("case: count GroupModules is 4, count Module is 6")
                    << QJsonDocument { { QJsonObject {{"downloads", array }}}} << modules << groupModules << true;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", ""}};
            modules.push_back(Module("name", "des", "abbr", m_idGroupModules));
            QTest::newRow("case: count GroupModules is 4, count Module is 7")
                    << QJsonDocument { { QJsonObject {{"downloads", array }}}} << modules << groupModules << true;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", ""}};
            modules.push_back(Module("name", "des", "abbr", m_idGroupModules));
            QTest::newRow("case: count GroupModules is 4, count Module is 8")
                    << QJsonDocument { { QJsonObject {{"downloads", array }}}} << modules << groupModules << true;

            m_idGroupModules = 4;
            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", "region"}};
            modules.push_back(Module("name", "des", "abbr", m_idGroupModules));
            QTest::newRow("case: count GroupModules is 4, count Module is 9")
                    << QJsonDocument { { QJsonObject {{"downloads", array }}}} << modules << groupModules << true;

            QTest::newRow("not valid data") << helperGetInvalidDocument() << std::vector<Module>{} << std::vector<GroupModulesShared>{} << false;
        }

        void tst_ManagerGroup::makeCollections()
        {
            qRegisterMetaType<std::vector<Module>>("std::vector<Module>");
            qRegisterMetaType<std::vector<GroupModulesShared>>("std::vector<GroupModulesShared>");

            QFETCH(QJsonDocument, document);
            QFETCH(std::vector<Module>, modules);
            QFETCH(std::vector<GroupModulesShared>, groupModules);
            QFETCH(bool, hit);

            ManagerGroup managerGroup;
            QSignalSpy spyMakeModulesSuccess(&managerGroup, &ManagerGroup::makeModulesSuccess);
            QSignalSpy spyError(&managerGroup, &ManagerGroup::error);

            managerGroup.makeCollections(document);

            QCOMPARE(spyMakeModulesSuccess.count(), int(hit));

            if (hit) {
                QList<QVariant> arguments = spyMakeModulesSuccess.takeFirst();
                const std::vector<Module>& modules_actual = arguments[0].value<std::vector<Module>>();
                QCOMPARE(modules_actual.size(), modules.size());
                QCOMPARE(modules_actual, modules);

                QCOMPARE(managerGroup.m_objects.size(), groupModules.size());
                QCOMPARE(std::equal(dereference_iterator(groupModules.begin()),
                           dereference_iterator(groupModules.end()),
                           MapValueIterator(managerGroup.m_objects.begin())
                           ), true);
            } else {
                QCOMPARE(spyError.count(), 1);
                QList<QVariant> arguments = spyError.takeFirst();
                QCOMPARE(arguments[0].toString(), QString("An error occured, please try in time."));
            }
        }

    }

}

QTEST_MAIN(modules::tests::tst_ManagerGroup)

#include "tst_managergroup.moc"
