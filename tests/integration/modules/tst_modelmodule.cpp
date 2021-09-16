#include <QtTest>
#include "modelmodule.h"
#include "modeljsontest.h"
#include "dereferenceiterator.h"
#include "quickdownload.h"
#include <chrono>

namespace modules {
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;

    namespace tests {

        using namespace sqlite_orm;

        class tst_ModelModule : public ::tests::ModelJsonTest<Module, ModelModule> {
            Q_OBJECT

        public:
            tst_ModelModule();
            ~tst_ModelModule();

        private:
            std::vector<ModuleShared> helperGetObjects() const override;
            std::vector<ModuleUnique> helperGetObjectsUnique() const override;
        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void update() override;
            void updateObjects_data();
            void updateObjects();
            void updateSelecting_data();
            void updateSelecting();
            void updateDownloaded_data();
            void updateDownloaded();
//            void getExtraFields();
            void getExtraFieldsFromDb();
            void saveExtraFieldsToDb();
            void saveExtraFieldsToDb_data();
            void downloadModules();
        };

        tst_ModelModule::tst_ModelModule() {}

        tst_ModelModule::~tst_ModelModule() {}

        void tst_ModelModule::initTestCase()
        {
            ModelJsonTest<Module, ModelModule>::initTestCase();
        }

        void tst_ModelModule::cleanupTestCase()
        {
            ModelJsonTest<Module, ModelModule>::cleanupTestCase();
        }

        //helpers
        std::vector<ModuleShared> tst_ModelModule::helperGetObjects() const
        {
            std::vector<ModuleShared> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(
                            std::make_shared<Module>(
                                "name",
                                "description",
                                QString("abbreviation_%1").arg(in),
                                0,
                                102400,
                                "en",
                                "information",
                                "comment",
                                "copyright",
                                QDate(2017, 03, 31),
                                false,
                                false)
                );
            }
            return objects;
        }

        std::vector<ModuleUnique> tst_ModelModule::helperGetObjectsUnique() const
        {
            std::vector<ModuleUnique> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(
                            std::make_unique<Module>(
                                "name",
                                "description",
                                "abbreviation",
                                0,
                                102400,
                                "en",
                                "information",
                                "comment",
                                "copyright",
                                QDate(2017, 03, 31),
                                false,
                                false)
                );
            }
            return objects;
        }

        // tests
        void tst_ModelModule::update()
        {
//            ModelJsonTest<Module, ModelModule>::update();
        }

        void tst_ModelModule::updateObjects_data()
        {
            cleanTable();
            helperSave();
        }

        void tst_ModelModule::updateObjects()
        {
            const auto &objects = helperGetObjectsUnique();

            ModelModule modelModule;
            modelModule.updateObjects();
            QCOMPARE(modelModule.m_objects.size(), objects.size());
//            QCOMPARE(std::equal(dereference_iterator(modelModule.m_objects.begin()),
//                       dereference_iterator(modelModule.m_objects.end()),
//                       dereference_iterator(objects.begin())
//                       ), true);
            QCOMPARE(modelModule.objectsCount, 0);
        }

        void tst_ModelModule::updateSelecting_data()
        {
            QTest::addColumn<bool>("value");

            QTest::newRow("m_selecting is true") << true;
            QTest::newRow("m_selecting is false") << false;
        }

        void tst_ModelModule::updateSelecting()
        {
            QFETCH(bool, value);

            cleanTable();
            helperSave();

            ModelModule model;
            int id = 1;
            model.updateSelected(id, value);
            const auto &object = m_db->storage->get_pointer<Module>(id);
            QCOMPARE(object->m_selected, value);
        }

        void tst_ModelModule::updateDownloaded_data()
        {
            QTest::addColumn<bool>("value");

            QTest::newRow("m_downloaded is true") << true;
            QTest::newRow("m_downloaded is false") << false;
        }

        void tst_ModelModule::updateDownloaded()
        {
            QFETCH(bool, value);

            cleanTable();
            helperSave();

            ModelModule model;
            int id = 1;
            model.updateDownloaded(id, value);
            const auto &object = m_db->storage->get_pointer<Module>(id);
            QCOMPARE(object->m_downloaded, value);
        }

        void tst_ModelModule::getExtraFieldsFromDb()
        {
            cleanTable();
            helperSave();

            m_db->storage->update_all(set(assign(&Module::m_downloaded, true)));
            m_db->storage->update_all(set(assign(&Module::m_selected, true)));

            ModelModule model;
            model.getExtraFieldsFromDb();

            QCOMPARE(model.selected->size(), vectorSize);
            QCOMPARE(model.downloaded->size(), vectorSize);
        }

        void tst_ModelModule::saveExtraFieldsToDb_data()
        {
            cleanTable();
            helperSave();

            ModelModule model;
            model.downloaded = std::make_unique<ModelModule::Downloaded> ();
            model.downloaded->push_back(std::make_tuple("abbreviation_0"));
            model.downloaded->push_back(std::make_tuple("abbreviation_2"));

            model.selected = std::make_unique<ModelModule::Downloaded> ();
            model.selected->push_back(std::make_tuple("abbreviation_0"));
            model.selected->push_back(std::make_tuple("abbreviation_2"));
            model.saveExtraFieldsToDb();

            QTest::addColumn<int>("id");
            QTest::addColumn<bool>("selected");
            QTest::addColumn<bool>("downloaded");

            QTest::newRow("abbreviation_0 m_downloaded and m_selected is true") << 1 << true << true;
            QTest::newRow("abbreviation_1 m_downloaded and m_selected is false") << 2 << false << false;
            QTest::newRow("abbreviation_2 m_downloaded and m_selected is true") << 3 << true << true;
            QCOMPARE(int(model.selected->size()), 0);
            QCOMPARE(int(model.downloaded->size()), 0);
        }

        void tst_ModelModule::saveExtraFieldsToDb()
        {
            QFETCH(int, id);
            QFETCH(bool, selected);
            QFETCH(bool, downloaded);

            const auto &object = m_db->storage->get_pointer<Module>(id);
            QCOMPARE(object->m_downloaded, downloaded);
            QCOMPARE(object->m_selected, selected);
        }

        void tst_ModelModule::downloadModules()
        {
//            std::unique_ptr<ModelModule::Downloaded> downloaded = std::make_unique<ModelModule::Downloaded> ();
//            downloaded->push_back(std::make_tuple("AGP"));
//            downloaded->push_back(std::make_tuple("ARC"));

            ModelModule model;
            Clock::time_point t0 = Clock::now();
            model.downloadModules({QUrl("https://sabnzbd.org/tests/internetspeed/50MB.bin"), QUrl("https://github.com/yourkin/fileupload-fastapi/raw/a85a697cab2f887780b3278059a0dd52847d80f3/tests/data/test-5mb.bin")});
            Clock::time_point t1 = Clock::now();
            milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
            qDebug() << ms.count() << "ms\n";

//            t0 = Clock::now();
//            QList<QUrl> urls {
////                QUrl("https://speed.hetzner.de/100MB.bin"),
//                QUrl("https://sabnzbd.org/tests/internetspeed/50MB.bin"),
////                QUrl("https://www.dundeecity.gov.uk/sites/default/files/publications/civic_renewal_forms.zip"),
////                QUrl("https://telegram.org/dl/desktop/mac")
//            };
//            QNetworkAccessManager manager;
//            QEventLoop loop;
//            int total = 1;

//            QFile file = QFile(QDir::currentPath() + "/modules/test_0");

//            if (!file.open(QIODevice::WriteOnly)) {
//                qDebug() << "sdsd" << file.errorString();
//            }

//            QNetworkReply *reply = manager.get(QNetworkRequest(urls[0]));
//            QObject::connect(reply, &QNetworkReply::finished, [&total, &loop](){
//                total--;
//                if(total == 0) {
//                    loop.quit();
//                }
//            });
//            QObject::connect(reply, &QNetworkReply::readyRead, [&file, &reply](){
//                file.write(reply->readAll());
//            });



////            QFile file_1 = QFile(QDir::currentPath() + "/modules/test_1");

////            if (!file_1.open(QIODevice::WriteOnly)) {
////                qDebug() << "sdsd" << file.errorString();
////            }

////            QNetworkReply *reply_1 = manager.get(QNetworkRequest(urls[1]));
////            QObject::connect(reply_1, &QNetworkReply::finished, [&total, &loop](){
////                total--;
////                if(total == 0) {
////                    loop.quit();
////                }
////            });
////            QObject::connect(reply_1, &QNetworkReply::readyRead, [&file_1, &reply_1](){
////                file_1.write(reply_1->readAll());
////            });

////            QFile file_2 = QFile(QDir::currentPath() + "/modules/test_2");

////            if (!file_2.open(QIODevice::WriteOnly)) {
////                qDebug() << "sdsd" << file.errorString();
////            }

////            QNetworkReply *reply_2 = manager.get(QNetworkRequest(urls[2]));
////            QObject::connect(reply_2, &QNetworkReply::finished, [&total, &loop](){
////                total--;
////                if(total == 0) {
////                    loop.quit();
////                }
////            });
////            QObject::connect(reply_2, &QNetworkReply::readyRead, [&file_2, &reply_2](){
////                file_2.write(reply_2->readAll());
////            });
//            loop.exec();
////            m_saveFile.commit();
//            t1 = Clock::now();
//            ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
//            qDebug() << ms.count() << "ms\n";
        }
    }
}

QTEST_MAIN(modules::tests::tst_ModelModule)

#include "tst_modelmodule.moc"
