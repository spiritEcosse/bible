#include <QtTest>
#include <JlCompress.h>
#include "managergroup.h"
#include "groupmodules.h"
#include "managerregistry.h"
#include "modelregistry.h"
#include "module.h"
#include "dereferenceiterator.h"
#include "modelgroupmodules.h"

Q_DECLARE_METATYPE(std::vector<modules::Module>)
Q_DECLARE_METATYPE(std::vector<modules::GroupModulesShared>)

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
            QJsonDocument helperGetInvalidDocument() const;

        private slots:
            void initTestCase();
            void cleanupTestCase();
            void makeCollections_data();
            void makeCollections();
            void downloadRegistry_data();
            void downloadRegistry();
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
            QSignalSpy spyMakeModulesSuccess(&managerGroup, &ManagerGroup::makeGroupModulesSuccess);
            QSignalSpy spyModelUpdate(managerGroup.m_modelModule.get(), &ModelModule::updateDone);

            managerGroup.m_managerRegistry->m_modelRegistry->m_objects.clear();
            managerGroup.m_managerRegistry->m_modelRegistry->m_objects.push_back(
                        std::make_unique<Registry>(
                            QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                            QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                        )
            );

            managerGroup.downloadRegistry();

            QVERIFY(spyMakeGroupModulesSuccess.wait());
            QCOMPARE(spy.count(), 1);
            QCOMPARE(spyMakeModulesSuccess.count(), 1);
            QCOMPARE(spyMakeGroupModulesSuccess.count(), 1);
            QCOMPARE(spyModelUpdate.count(), 1);
        }

    }

}

QTEST_MAIN(modules::tests::tst_ManagerGroup)

#include "tst_managergroup.moc"
