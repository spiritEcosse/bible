#include <QtTest>
#include "modelmodule.h"
#include "modeljsontest.h"
#include "dereferenceiterator.h"
#include "quickdownload.h"
#include <chrono>
#include <thread>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <fstream>

static const char *urls[] = {
  "https://speed.hetzner.de/100MB.bin",
  "https://sabnzbd.org/tests/internetspeed/50MB.bin",
  "https://opensource.org",
  "https://www.google.com",
  "https://www.yahoo.com",
  "https://www.ibm.com",
  "https://www.mysql.com",
  "https://www.oracle.com",
  "https://www.ripe.net",
  "https://www.iana.org",
  "https://www.amazon.com",
  "https://www.netcraft.com",
  "https://www.heise.de",
  "https://www.chip.de",
  "https://www.ca.com",
  "https://www.cnet.com",
  "https://www.mozilla.org",
  "https://www.cnn.com",
  "https://www.wikipedia.org",
  "https://www.dell.com",
  "https://www.hp.com",
  "https://www.cert.org",
  "https://www.mit.edu",
  "https://www.nist.gov",
  "https://www.ebay.com",
  "https://www.playstation.com",
  "https://www.uefa.com",
  "https://www.ieee.org",
  "https://www.apple.com",
  "https://www.symantec.com",
  "https://www.zdnet.com",
  "https://www.fujitsu.com/global/",
  "https://www.supermicro.com",
  "https://www.hotmail.com",
  "https://www.ietf.org",
  "https://www.bbc.co.uk",
  "https://news.google.com",
  "https://www.foxnews.com",
  "https://www.msn.com",
  "https://www.wired.com",
  "https://www.sky.com",
  "https://www.usatoday.com",
  "https://www.cbs.com",
  "https://www.nbc.com/",
  "https://slashdot.org",
  "https://www.informationweek.com",
  "https://apache.org",
  "https://www.un.org",
};

#define MAX_PARALLEL 10 /* number of simultaneous transfers */
#define NUM_URLS sizeof(urls)/sizeof(char *)


using MultiHandle = std::unique_ptr<CURLM, std::function<void(CURLM*)>>;
MultiHandle CreateMultiHandle()
{
    auto curl = MultiHandle(curl_multi_init(), curl_multi_cleanup);
    if (!curl)
    {
        throw std::runtime_error("Failed creating CURL multi object");
    }
    return curl;
}

using EasyHandle = std::unique_ptr<CURL, std::function<void(CURL*)>>;
EasyHandle CreateEasyHandle()
{
    auto curl = EasyHandle(curl_easy_init(), curl_easy_cleanup);
    if (!curl)
    {
        throw std::runtime_error("Failed creating CURL easy object");
    }
    return curl;
}

int wait_if_needed(CURLM* multi_handle, timeval& timeout)
{
    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);
    int maxfd = -1;
    /* get file descriptors from the transfers */
    auto mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

    if (mc != CURLM_OK) {
        qDebug() << "curl_multi_fdset() failed, code " << mc << ".";
    }
    /* On success the value of maxfd is guaranteed to be >= -1. We call
           sleep for 100ms, which is the minimum suggested value in the
           curl_multi_fdset() doc. */
    if (maxfd == -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    int rc = maxfd != -1 ? select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout) : 0;
    return rc;
}

timeval get_timeout(CURLM* multi_handle)
{
    long curl_timeo = -1;
    /* set a suitable timeout to play around with */
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    curl_multi_timeout(multi_handle, &curl_timeo);
    if (curl_timeo >= 0) {
        timeout.tv_sec = curl_timeo / 1000;
        if (timeout.tv_sec > 1)
            timeout.tv_sec = 1;
        else
            timeout.tv_usec = (curl_timeo % 1000) * 1000;
    }
    return timeout;
}

void multi_loop(CURLM* multi_handle)
{
    int still_running = 0; /* keep number of running handles */

    /* we start some action by calling perform right away */
    curl_multi_perform(multi_handle, &still_running);

    while (still_running) {
        struct timeval timeout = get_timeout(multi_handle);

        auto rc = wait_if_needed(multi_handle, timeout);

        if (rc >= 0)
        {
            /* timeout or readable/writable sockets */
            curl_multi_perform(multi_handle, &still_running);
        }
        /* else select error */
    }
}

size_t write_to_file(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    auto file = reinterpret_cast<std::ofstream*>(userp);
    file->write(reinterpret_cast<const char*>(contents), realsize);
    return realsize;
}

void set_ssl(CURL* curl)
{
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
}


void save_to_file(CURL* curl, int i)
{
    static std::ofstream file("downloaded_data_" + std::to_string(i), std::ios::binary);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(&file));
}

int download_asynchronous(void)
{
    std::vector<char*> urls = {"https://github.com/yourkin/fileupload-fastapi/raw/a85a697cab2f887780b3278059a0dd52847d80f3/tests/data/test-5mb.bin", "https://www.dundeecity.gov.uk/sites/default/files/publications/civic_renewal_forms.zip"};
    std::list<EasyHandle> handles(urls.size());
    MultiHandle multi_handle;

    /* init easy and multi stacks */
    try
    {
        multi_handle = CreateMultiHandle();
        std::for_each(handles.begin(), handles.end(), [](auto& handle){handle = CreateEasyHandle(); });
    }
    catch (const std::exception& ex)
    {
        qDebug() << ex.what();
        return -1;
    }
    /* set options */
    int i = 0;
    std::for_each(handles.begin(), handles.end(), [&i, &urls](auto& handle) {
        curl_easy_setopt(handle.get(), CURLOPT_URL, urls[i]);
        set_ssl(handle.get());
        save_to_file(handle.get(), i);
        qDebug() << urls[i] << i;
        i++;
    });

    /* add the individual transfers */
    std::for_each(handles.begin(), handles.end(), [&multi_handle](auto& handle) {curl_multi_add_handle(multi_handle.get(), handle.get()); });

    multi_loop(multi_handle.get());

    std::for_each(handles.begin(), handles.end(), [&multi_handle](auto& handle) {curl_multi_remove_handle(multi_handle.get(), handle.get()); });
    return 0;
} //It was originally published on https://www.apriorit.com/

namespace modules {

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
            download_asynchronous();
//            std::unique_ptr<ModelModule::Downloaded> downloaded = std::make_unique<ModelModule::Downloaded> ();
//            downloaded->push_back(std::make_tuple("AGP"));
//            downloaded->push_back(std::make_tuple("ARC"));

//            ModelModule model;
//            model.downloadModules(downloaded);
        }
    }
}

QTEST_MAIN(modules::tests::tst_ModelModule)

#include "tst_modelmodule.moc"
