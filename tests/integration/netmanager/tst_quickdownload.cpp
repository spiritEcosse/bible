#include <QtTest>
#include "quickdownload.h"

namespace netmanager {

    namespace tests {

        class tst_QuickDownload : public QObject  {
            Q_OBJECT

        public:
            tst_QuickDownload();
            ~tst_QuickDownload();
        private:
            const size_t vectorSize = 3;
            virtual std::vector<modules::HostUnique> helperGetHostsUnique() const;

        private slots:
            void initTestCase();
            void cleanupTestCase();
            void makeUrl();
            void setDestination();
        };

        tst_QuickDownload::tst_QuickDownload() {}

        tst_QuickDownload::~tst_QuickDownload() {}

        void tst_QuickDownload::initTestCase() {}

        void tst_QuickDownload::cleanupTestCase() {}

        //helpers

        std::vector<modules::HostUnique> tst_QuickDownload::helperGetHostsUnique() const
        {
            std::vector<modules::HostUnique> objects;
            for ( size_t in = 0; in < vectorSize; in++) {
                objects.push_back(std::make_unique<modules::Host>("alias", "aHR0cDovL2RvbWFpbi5jb20vdGVzdC8lcy56aXA=", 1, 2));
            }
            return objects;
        }

        // tests

        void tst_QuickDownload::makeUrl()
        {
            QuickDownload download;
            download._moduleName = "abbr";
            download.m_modelHost->m_objects = helperGetHostsUnique();
            download.makeUrl();

            QCOMPARE(download._url, QUrl("http://domain.com/test/abbr.zip"));
        }

        void tst_QuickDownload::setDestination()
        {
            QString moduleName = "name";
            QuickDownload download;
            download._moduleName = moduleName;
            download.setDestination();
            QCOMPARE(download._destination, QDir::currentPath() + "/modules/" + moduleName + ".zip");
        }

    }

}

QTEST_MAIN(netmanager::tests::tst_QuickDownload)

#include "tst_quickdownload.moc"
