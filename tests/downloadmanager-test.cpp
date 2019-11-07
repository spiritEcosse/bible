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
#include "mock_qfileinfo.h"
#include "mock_qstringlist.h"
#include "mock_qnetworkrequest.h"
#include "mock_textprogressbar.h"


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
      mockDownloadManager.qFileInfo = &mockQFileInfo;
      mockDownloadManager.fileNames = &mockQStringList;
      mockDownloadManager.progressBar = &mockTextProgressBar;
  }

  void TearDown() override {
  }

  // Objects declared here can be used by all tests in the test case for Foo.
//  StrictMock<MockDownloadManager> mockDownloadManager;
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
  MockQFileInfo mockQFileInfo;
  MockQStringList mockQStringList;
  MockQNetworkRequest mockQNetworkRequest;
  MockTextProgressBar mockTextProgressBar;

  const QUrl url = BuiltInDefaultValue<const QUrl>::Get();
  const QStringList urls = {"url1"};
  QString filename = BuiltInDefaultValue<QString>::Get();
  QString path = BuiltInDefaultValue<QString>::Get();
  QString basename = BuiltInDefaultValue<QString>::Get();
  int statusCode = BuiltInDefaultValue<int>::Get();
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
        EXPECT_CALL(mockQTimer, singleShot(0, downloadManager, _)); // WARNING: add SLOT
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
//                .WillOnce(Return(url)); # WARNING: add Return(url)
        EXPECT_CALL(mockDownloadManager, append(_)); // pass url instead _
        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(true));
        EXPECT_CALL(mockQTimer, singleShot(0, downloadManager, _)); // WARNING: add SIGNAL
    }
    mockDownloadManager.appendUrls(urls);
}

TEST_F(DownloadManagerTest, saveFileName)
{
    ON_CALL(mockDownloadManager, saveFileName(_))
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentSaveFileName)
                );

    {
        InSequence s;
        EXPECT_CALL(mockQurl, path(QUrl::FullyDecoded))
                .WillOnce(Return(path));
        EXPECT_CALL(mockQFileInfo, setFile(path));
        EXPECT_CALL(mockQFileInfo, fileName())
                .WillOnce(Return(basename));
    }

    mockDownloadManager.saveFileName(mockQurl);
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
        EXPECT_CALL(mockQFile, fileName())
                .WillOnce(Return(filename));
        EXPECT_CALL(mockQStringList, append(filename));
    }

    mockDownloadManager.startNextDownload();
}

TEST_F(DownloadManagerTest, downloadProgress)
{

}

TEST_F(DownloadManagerTest, downloadFinishedError)
{
    ON_CALL(mockDownloadManager, downloadFinished())
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentDownloadFinished)
                );
    {
        InSequence s;
        EXPECT_CALL(mockTextProgressBar, clear());
        EXPECT_CALL(mockQFile, close());
        EXPECT_CALL(mockQNetworkReply, error())
                .WillOnce(Return(QNetworkReply::NetworkError::TimeoutError));
        EXPECT_CALL(mockQFile, remove());
        EXPECT_CALL(mockQNetworkReply, deleteLater());
        EXPECT_CALL(mockDownloadManager, startNextDownload());
    }

    mockDownloadManager.downloadFinished();
}

TEST_F(DownloadManagerTest, downloadFinished)
{
    ON_CALL(mockDownloadManager, downloadFinished())
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentDownloadFinished)
                );

    {
        InSequence s;
        EXPECT_CALL(mockTextProgressBar, clear());
        EXPECT_CALL(mockQFile, close());
        EXPECT_CALL(mockQNetworkReply, error())
                .WillOnce(Return(QNetworkReply::NetworkError::NoError));
        EXPECT_CALL(mockDownloadManager, isHttpRedirect())
                .WillOnce(Return(true));
        EXPECT_CALL(mockDownloadManager, reportRedirect());
        EXPECT_CALL(mockQFile, remove());
        EXPECT_CALL(mockQNetworkReply, deleteLater());
        EXPECT_CALL(mockDownloadManager, startNextDownload());
    }

    mockDownloadManager.downloadFinished();
}

TEST_F(DownloadManagerTest, downloadFinishedNotRedirect)
{
    ON_CALL(mockDownloadManager, downloadFinished())
            .WillByDefault(
                Invoke(&mockDownloadManager, &MockDownloadManager::parentDownloadFinished)
                );

    {
        InSequence s;
        EXPECT_CALL(mockTextProgressBar, clear());
        EXPECT_CALL(mockQFile, close());
        EXPECT_CALL(mockQNetworkReply, error())
                .WillOnce(Return(QNetworkReply::NetworkError::NoError));
        EXPECT_CALL(mockDownloadManager, isHttpRedirect())
                .WillOnce(Return(false));
    }

    mockDownloadManager.downloadFinished();
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
    ON_CALL(mockDownloadManager, reportRedirect())
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentReportRedirect)
                );

    MockQVariant mockQVariantTarget;
    {
        InSequence s;

        EXPECT_CALL(mockQNetworkReply, attribute(QNetworkRequest::HttpStatusCodeAttribute))
                .WillOnce(ReturnPointee(&mockQVariant));
        EXPECT_CALL(mockQVariant, toInt(nullptr))
                .WillOnce(Return(statusCode));
        EXPECT_CALL(mockQNetworkReply, request())
                .WillOnce(ReturnPointee(&mockQNetworkRequest));
        EXPECT_CALL(mockQNetworkRequest, url())
                .WillOnce(ReturnPointee(&mockQurl));
        EXPECT_CALL(mockQurl, toDisplayString(QUrl::FormattingOptions(QUrl::PrettyDecoded)));
        EXPECT_CALL(mockQNetworkReply, attribute(QNetworkRequest::RedirectionTargetAttribute))
                .WillOnce(ReturnPointee(&mockQVariantTarget));
        EXPECT_CALL(mockQVariantTarget, isValid())
                .WillOnce(Return(false));
    }

    mockDownloadManager.reportRedirect();
}

TEST_F(DownloadManagerTest, reportRedirectTargetNotValid)
{
    ON_CALL(mockDownloadManager, reportRedirect())
            .WillByDefault(
                Invoke(&mockDownloadManager, &MockDownloadManager::parentReportRedirect)
                );

    MockQVariant mockQVariantTarget;
    MockQUrl mockQurlRedirect;
    {
        InSequence s;

        EXPECT_CALL(mockQNetworkReply, attribute(QNetworkRequest::HttpStatusCodeAttribute))
                .WillOnce(ReturnPointee(&mockQVariant));
        EXPECT_CALL(mockQVariant, toInt(nullptr))
                .WillOnce(Return(statusCode));
        EXPECT_CALL(mockQNetworkReply, request())
                .WillOnce(ReturnPointee(&mockQNetworkRequest));
        EXPECT_CALL(mockQNetworkRequest, url())
                .WillOnce(ReturnRef(mockQurl));
        EXPECT_CALL(mockQurl, toDisplayString(QUrl::FormattingOptions(QUrl::PrettyDecoded)));
        EXPECT_CALL(mockQNetworkReply, attribute(QNetworkRequest::RedirectionTargetAttribute))
                .WillOnce(ReturnPointee(&mockQVariantTarget));
        EXPECT_CALL(mockQVariantTarget, isValid())
                .WillOnce(Return(true));
        EXPECT_CALL(mockQVariantTarget, toUrl())
                .WillOnce(ReturnRef(mockQurlRedirect));
        EXPECT_CALL(mockQurlRedirect, isRelative());
    }

    mockDownloadManager.reportRedirect();
}
