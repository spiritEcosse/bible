#include <QtTest>

#include "downloadmanager.h"

namespace tests {

        class tst_DownloadManager : public QObject
        {
            Q_OBJECT

        public:
            tst_DownloadManager();
            ~tst_DownloadManager();

        private slots:
        };

        tst_DownloadManager::tst_DownloadManager()
        {
        }

        tst_DownloadManager::~tst_DownloadManager()
        {

        }

}

QTEST_MAIN(tests::tst_DownloadManager)

#include "tst_downloadmanager.moc"
