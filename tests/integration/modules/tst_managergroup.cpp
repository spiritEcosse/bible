#include <QtTest>
#include <JlCompress.h>
#include "managergroup.h"
#include "groupmodules.h"
#include "managerregistry.h"
#include "modelregistry.h"
#include "module.h"

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::vector<modules::Module>)
Q_DECLARE_METATYPE(std::vector<modules::GroupModules>)
Q_DECLARE_METATYPE(QJsonArray)

namespace modules {

    namespace tests {

        class tst_ManagerGroup : public QObject
        {
            Q_OBJECT

        public:
            tst_ManagerGroup();
            ~tst_ManagerGroup();

        private:
            const QString pathFiles { "files" };
            const QString dirDownload = "download";
            const QString strUrl { "file://" };
            QFile fileRegistry { "registry.json" };
            const QFile fileRegistryArchive { "registry.zip" };
            const QFile fileRegistryInfo { "registry_info.json" };
            QDir dir;
            void initTestCase();
            void cleanupTestCase();

        private slots:
            void makeCollections_data();
            void makeCollections();
            void downloadRegistry_data();
            void downloadRegistry();
        };

        tst_ManagerGroup::tst_ManagerGroup()
        {

        }

        tst_ManagerGroup::~tst_ManagerGroup()
        {

        }

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

        void tst_ManagerGroup::makeCollections_data()
        {
            QTest::addColumn<QJsonDocument>("document");
            QTest::addColumn<std::vector<Module>>("modules");
            QTest::addColumn<std::vector<GroupModules>>("groupModules");

            std::vector<Module> modules;
            std::vector<GroupModules> groupModules;
            QJsonArray array;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "en"}};
            modules.push_back(Module("name", "des", "abbr", 1));
            groupModules.push_back(GroupModules("en", "Name"));
            QTest::newRow("case: count GroupModules is 1, count Module is 1")
                    << QJsonDocument { QJsonObject {{"downloads", array }}} << modules << groupModules;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "en"}};
            modules.push_back(Module("name", "des", "abbr", 1));
            QTest::newRow("case: count GroupModules is 1, count Module is 2")
                    << QJsonDocument { QJsonObject {{"downloads", array }}} << modules << groupModules;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "en"},{"reg", "region"}};
            modules.push_back(Module("name", "des", "abbr", 2));
            groupModules.insert(groupModules.begin(), GroupModules("en", "Name", "region"));
            QTest::newRow("case: count GroupModules is 2, count Module is 3")
                    << QJsonDocument { QJsonObject {{"downloads", array }}} << modules << groupModules;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", ""}};
            modules.push_back(Module("name", "des", "abbr", 3));
            groupModules.insert(groupModules.begin(), GroupModules("av", "Name"));
            QTest::newRow("case: count GroupModules is 3, count Module is 4")
                    << QJsonDocument { QJsonObject {{"downloads", array }}} << modules << groupModules;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", "region"}};
            modules.push_back(Module("name", "des", "abbr", 4));
            groupModules.insert(groupModules.begin(), GroupModules("av", "Name", "region"));
            QTest::newRow("case: count GroupModules is 4, count Module is 5")
                    << QJsonDocument { { QJsonObject {{"downloads", array }}}} << modules << groupModules;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", ""}};
            modules.push_back(Module("name", "des", "abbr", 4));
            QTest::newRow("case: count GroupModules is 4, count Module is 6")
                    << QJsonDocument { { QJsonObject {{"downloads", array }}}} << modules << groupModules;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", ""}};
            modules.push_back(Module("name", "des", "abbr", 4));
            QTest::newRow("case: count GroupModules is 4, count Module is 7")
                    << QJsonDocument { { QJsonObject {{"downloads", array }}}} << modules << groupModules;

            array << QJsonObject {{"fil", "name"},{"des", "des"},{"abr", "abbr"},{"lng", "av"},{"reg", ""}};
            modules.push_back(Module("name", "des", "abbr", 4));
            QTest::newRow("case: count GroupModules is 4, count Module is 8")
                    << QJsonDocument { { QJsonObject {{"downloads", array }}}} << modules << groupModules;
        }

        void tst_ManagerGroup::makeCollections()
        {
            qRegisterMetaType<std::vector<Module>>("std::vector<Module>");
            qRegisterMetaType<std::vector<GroupModules>>("std::vector<GroupModules>");

            QFETCH(QJsonDocument, document);
            QFETCH(std::vector<Module>, modules);
            QFETCH(std::vector<GroupModules>, groupModules);

            ManagerGroup managerGroup;
            QSignalSpy spyMakeModulesSuccess(&managerGroup, &ManagerGroup::makeModulesSuccess);
            QSignalSpy spyMakeGroupModulesSuccess(&managerGroup, &ManagerGroup::makeGroupModulesSuccess);

            managerGroup.makeCollections(document);

            QCOMPARE(spyMakeModulesSuccess.count(), 1);
            QCOMPARE(spyMakeGroupModulesSuccess.count(), 1);

            QList<QVariant> arguments = spyMakeModulesSuccess.takeFirst();
            const std::vector<Module>& modules_actual = arguments[0].value<std::vector<Module>>();
            QCOMPARE(modules_actual.size(), modules.size());
            QCOMPARE(modules_actual, modules);

            arguments = spyMakeGroupModulesSuccess.takeFirst();
            const std::vector<GroupModules>& groupModules_actual = arguments[0].value<std::vector<GroupModules>>();
            QCOMPARE(groupModules_actual.size(), groupModules.size());
            QCOMPARE(groupModules_actual, groupModules);
        }

        void tst_ManagerGroup::downloadRegistry_data() {
            QSettings settings;
            settings.setValue("registryVersion", 0);

            fileRegistry.open(QFile::WriteOnly);
            fileRegistry.write(
                        QJsonDocument {
                            QJsonObject {
                                {
                                    "downloads",
                                    QJsonArray {
                                        QJsonObject {
                                            {"fil", "name"},
                                            {"des", "description"},
                                            {"abr", "abbreviation"}
                                        }
                                    },
                                },
                                {"version", 1}
                            }
                        }.toJson());
            fileRegistry.close();

            QVERIFY(JlCompress::compressFile(fileRegistryArchive.fileName(), fileRegistry.fileName()));
        }

        void tst_ManagerGroup::downloadRegistry()
        {
            ManagerGroup managerGroup;
            QSignalSpy spy(managerGroup.m_managerRegistry.get(), &ManagerRegistry::retrieveDataSuccess);
            QSignalSpy spyMakeGroupModulesSuccess(&managerGroup, &ManagerGroup::makeGroupModulesSuccess);

            managerGroup.m_managerRegistry->m_registry.reset(
                        new Registry {
                            QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                            QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                        });

            managerGroup.downloadRegistry();

            QVERIFY(spyMakeGroupModulesSuccess.wait(10000));
            QCOMPARE(spy.count(), 1);
            QCOMPARE(spyMakeGroupModulesSuccess.count(), 1);
        }

    }

}

QTEST_MAIN(modules::tests::tst_ManagerGroup)

#include "tst_managergroup.moc"
