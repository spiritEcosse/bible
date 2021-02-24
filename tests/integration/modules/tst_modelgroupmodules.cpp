#include <QtTest>
#include "modelgroupmodules.h"
#include "groupmodules.h"
#include <JlCompress.h>
#include "basetest.h"

namespace modules {

    namespace tests {

        class tst_ModelGroupModules :  public ::tests::BaseTest<GroupModules, ModelGroupModules> {
            Q_OBJECT

        private:
            const QString strUrl { "file://" };
            QFile fileRegistry { "registry.json" };
            const QFile fileRegistryArchive { "registry.zip" };
            const QFile fileRegistryInfo { "registry_info.json" };
            QJsonDocument helperGetInvalidDocument() const;
            void setQSettings(int value = 0, QString key = "registryVersion");

        public:
            tst_ModelGroupModules();
            ~tst_ModelGroupModules();

        private:
            std::vector<GroupModules> helperGetObjects() const override;

        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void update() override;
            void contructor_data();
            void contructor();
            void newVersionAvailable();
            void updateCompleted();
            void downloadRegistry_data();
            void downloadRegistry();
        };

        tst_ModelGroupModules::tst_ModelGroupModules() {}

        tst_ModelGroupModules::~tst_ModelGroupModules() {}

        void tst_ModelGroupModules::initTestCase()
        {
            ::tests::BaseTest<GroupModules, ModelGroupModules>::initTestCase();
        }

        void tst_ModelGroupModules::cleanupTestCase()
        {
            ::tests::BaseTest<GroupModules, ModelGroupModules>::cleanupTestCase();
        }

        //helpers

        void tst_ModelGroupModules::setQSettings(int value, QString key)
        {
            QSettings settings;
            settings.setValue(key, value);
        }

        std::vector<GroupModules> tst_ModelGroupModules::helperGetObjects() const
        {
            return std::vector<GroupModules> {vectorSize, {"en", "name", "region"}};
        }

        // tests
        void tst_ModelGroupModules::contructor_data()
        {
            QTest::addColumn<bool>("available");
            QTest::addColumn<int>("version");

            QTest::newRow("available new version is true") << true << 11;
            QTest::newRow("available new version is false") << false << 0;
        }

        void tst_ModelGroupModules::contructor()
        {
            QFETCH(bool, available);
            QFETCH(int, version);
            setQSettings(version, "cacheRegistryVersion");

            ModelGroupModules modelGroupModules;
            QCOMPARE(modelGroupModules.m_newVersionAvailable, available);
        }

        void tst_ModelGroupModules::newVersionAvailable()
        {
            ModelGroupModules model;
            QCOMPARE(model.newVersionAvailable(), false);
        }

        void tst_ModelGroupModules::updateCompleted()
        {
            ModelGroupModules model;
            QCOMPARE(model.updateCompleted(), false);
        }

        void tst_ModelGroupModules::update()
        {
            ::tests::BaseTest<GroupModules, ModelGroupModules>::update();
        }

        void tst_ModelGroupModules::downloadRegistry_data() {
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

        void tst_ModelGroupModules::downloadRegistry()
        {
            ModelGroupModules modelGroupModules;
            QSignalSpy spy(&modelGroupModules, &ModelGroupModules::updateDone);
            QSignalSpy spyChangeNewVersionAvailable(&modelGroupModules, &ModelGroupModules::changeNewVersionAvailable);
            QSignalSpy spyChangeUpdateCompleted(&modelGroupModules, &ModelGroupModules::changeUpdateCompleted);

            modelGroupModules.m_managerGroup->m_managerRegistry->m_registry.reset(
                        new Registry {
                            QString(strUrl + QFileInfo(fileRegistryArchive).absoluteFilePath()).toUtf8().toBase64(),
                            QString(strUrl + QFileInfo(fileRegistryInfo).absoluteFilePath()).toUtf8().toBase64()
                        });

            modelGroupModules.downloadRegistry();
            QCOMPARE(modelGroupModules.m_newVersionAvailable, false);
            QVERIFY(spy.wait());
            QCOMPARE(spy.count(), 1);
            QCOMPARE(modelGroupModules.m_updateCompleted, true);
            QCOMPARE(spyChangeUpdateCompleted.count(), 1);
            QCOMPARE(spyChangeNewVersionAvailable.count(), 1);
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelGroupModules)

#include "tst_modelgroupmodules.moc"
