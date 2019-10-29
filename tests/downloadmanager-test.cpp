#include "gtest_global.h"

#include "../src/ModulesGroupModel.h"

#include "mock_downloadmanager.h"
#include "mock_modulesgroupmodel.h"
#include "mock_qtimer.h"
#include "mock_qqueue.h"
#include "mock_qurl.h"
#include "mock_qfile.h"
#include "mock_qnetworkreply.h"
#include "mock_qvariant.h"


class DownloadManagerTest : public ::testing::Test
{
protected:
  DownloadManagerTest()
      : downloadManager(&mockDownloadManager),
        modulesGroupModel(&mockModulesGroupModel)
  {}

  ~DownloadManagerTest() override {}

  void SetUp() override {
      mockDownloadManager.timer = &mockQTimer;
      mockDownloadManager.downloadQueue = &mockQqueue;
      mockDownloadManager.qurl = &mockQurl;
      mockDownloadManager.output = &mockQFile;
      mockDownloadManager.currentDownload = &mockQNetworkReply;
  }

  void TearDown() override {
  }

  // Objects declared here can be used by all tests in the test case for Foo.
  MockDownloadManager mockDownloadManager;
  DownloadManager* downloadManager;

  MockModulesGroupModel mockModulesGroupModel;
  ModulesGroupModel* modulesGroupModel;

  MockQQueue<QUrl> mockQqueue;
  MockQTimer mockQTimer;
  MockQUrl mockQurl;
  MockQFile mockQFile;
  MockQNetworkReply mockQNetworkReply;
  MockQVariant mockQVariant;

  const QUrl url = BuiltInDefaultValue<const QUrl>::Get();
  const QStringList urls = {"url1"};
  QString filename = BuiltInDefaultValue<QString>::Get();
};


TEST_F(DownloadManagerTest, append)
{
    ON_CALL(mockDownloadManager, append(url))
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentAppend)
                );

    EXPECT_EQ(NULL, mockDownloadManager.totalCount);

    {
        InSequence s;

        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(true));
        EXPECT_CALL(mockQTimer, singleShot(0, downloadManager, _)); // ToDo : add SLOT
        EXPECT_CALL(mockQqueue, enqueue(url));
    }
    mockDownloadManager.append(url);
    EXPECT_EQ(1, mockDownloadManager.totalCount);
}

TEST_F(DownloadManagerTest, appendUrls)
{
    ON_CALL(mockDownloadManager, appendUrls(urls))
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentAppendUrls)
                );

    {
        InSequence s;

        EXPECT_CALL(mockQurl, fromEncodedImpl(_, QUrl::TolerantMode));
//                .WillOnce(Return(url)); # ToDo add Return(url)
        EXPECT_CALL(mockDownloadManager, append(_)); // pass url instead _
        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(true));
        EXPECT_CALL(mockQTimer, singleShot(0, downloadManager, _)); // ToDo : add SIGNAL
    }
    mockDownloadManager.appendUrls(urls);
}

TEST_F(DownloadManagerTest, startNextDownload)
{
    QObject::connect(downloadManager, &DownloadManager::successfully,
                     modulesGroupModel, &ModulesGroupModel::decompressRegistry);
    QObject::connect(downloadManager, &DownloadManager::finished,
                     &DownloadManager::downloadFinished);

    ON_CALL(mockDownloadManager, startNextDownload())
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentStartNextDownload)
                );

    {
        InSequence s;

        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(true));
        EXPECT_CALL(mockDownloadManager, downloadFinished());
        EXPECT_EQ(mockDownloadManager.downloadedCount, mockDownloadManager.totalCount);
        EXPECT_CALL(mockModulesGroupModel, decompressRegistry());
    }

    mockDownloadManager.startNextDownload();

    {
        InSequence s;

        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(false));
        EXPECT_CALL(mockQqueue, dequeue())
                .WillOnce(ReturnPointee(&url));
        EXPECT_CALL(mockDownloadManager, saveFileName(_))
                .WillOnce(Return(filename));
        EXPECT_CALL(mockQFile, setFileName(filename));
        EXPECT_CALL(mockQFile, fileName());
    }

    QStringList fileNames;
    EXPECT_EQ(mockDownloadManager.fileNames, fileNames);
    mockDownloadManager.startNextDownload();
    fileNames.append(filename);
    EXPECT_EQ(mockDownloadManager.fileNames, fileNames);
}

TEST_F(DownloadManagerTest, downloadProgress)
{

}

TEST_F(DownloadManagerTest, downloadFinished)
{

}

TEST_F(DownloadManagerTest, downloadReadyRead)
{
    ON_CALL(mockDownloadManager, downloadReadyRead())
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentDownloadReadyRead)
                );

    QByteArray array;
    {
        InSequence s;
        
        EXPECT_CALL(mockQNetworkReply, readAll())
                .WillOnce(Return(array));
        EXPECT_CALL(mockQFile, write(array));
    }

    mockDownloadManager.downloadReadyRead();
}

TEST_F(DownloadManagerTest, isHttpRedirect)
{
    ON_CALL(mockDownloadManager, isHttpRedirect())
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentIsHttpRedirect)
                );

    {
        InSequence s;

        EXPECT_CALL(mockQNetworkReply, attribute(QNetworkRequest::HttpStatusCodeAttribute))
                .WillOnce(ReturnPointee(&mockQVariant));
        EXPECT_CALL(mockQVariant, toInt(nullptr))
                .WillOnce(Return(301));
    }
    EXPECT_TRUE(mockDownloadManager.isHttpRedirect());
}

TEST_F(DownloadManagerTest, reportRedirect)
{

}

TEST_F(DownloadManagerTest, saveFileName)
{

}
