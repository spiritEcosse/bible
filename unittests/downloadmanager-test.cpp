#include "gtest_global.h"

#include "mock_downloadmanager.h"
#include "mock_modulesgroupmodel.h"
#include "mock_qtimer.h"
#include "mock_qqueue.h"
#include "mock_qurl.h"
#include "mock_qstring.h"
#include "mock_qfile.h"
#include "mock_qnetworkreply.h"
#include "mock_qvariant.h"
#include "mock_qfileinfo.h"
#include "mock_qstringlist.h"
#include "mock_qnetworkrequest.h"
#include "mock_textprogressbar.h"
#include "mock_qnetworkaccessmanager.h"
#include "mock_qtime.h"

class DownloadManagerTest : public TestWithParam<int>
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
      mockDownloadManager.request = &mockQNetworkRequest;
      mockDownloadManager.progressBar = &mockTextProgressBar;
      mockDownloadManager.manager = &mockQNetworkAccessManager;
      mockDownloadManager.downloadTime = &mockQTime;
      mockDownloadManager.qString = &mockQString;
      mockDownloadManager.baseName = &baseName;
  }

  void TearDown() override {
  }

  // Objects declared here can be used by all tests in the test case for Foo.
  StrictMock<MockDownloadManager> mockDownloadManager;
  DownloadManager* downloadManager;
  StrictMock<MockQString> baseName;

  MockModulesGroupModel mockModulesGroupModel;
  ModulesGroupModel* modulesGroupModel;

  MockQQueue<QUrl> mockQqueue;
  MockQTimer mockQTimer;
  MockQUrl mockQurl;
  StrictMock<MockQString> mockQString;

  MockQFile mockQFile;
  MockQNetworkReply mockQNetworkReply;
  MockQNetworkAccessManager mockQNetworkAccessManager;
  MockQVariant mockQVariant;
  MockQFileInfo mockQFileInfo;
  MockQStringList mockQStringList;
  MockQNetworkRequest mockQNetworkRequest;
  MockTextProgressBar mockTextProgressBar;
  MockQTime mockQTime;

  const QUrl url = BuiltInDefaultValue<const QUrl>::Get();
  QList<QString*> urls = {&mockQString};
  QString filename = BuiltInDefaultValue<QString>::Get();
  QString path = BuiltInDefaultValue<QString>::Get();
  QString basename = BuiltInDefaultValue<QString>::Get();
  int statusCode = BuiltInDefaultValue<int>::Get();
  int bytesReceived = BuiltInDefaultValue<int>::Get();
  int bytesTotal = BuiltInDefaultValue<int>::Get();
  QByteArray qByteArray = BuiltInDefaultValue<QByteArray>::Get();
  const QString message = BuiltInDefaultValue<QString>::Get();
  QFile::OpenMode qFileWriteMode = QFile::WriteOnly;
  QUrl::ParsingMode parsingMode = QUrl::ParsingMode::TolerantMode;
};

static QMap<int, bool> codes = {
    { 301, true },
    { 302, true },
    { 303, true },
    { 305, true },
    { 307, true },
    { 308, true },
    { 306, false },
    { 309, false },
    { 310, false },
    { 200, false },
    { 400, false },
    { 500, false },
};

TEST_P(DownloadManagerTest, isHttpRedirectParam) {
    EXPECT_CALL(mockDownloadManager, isHttpRedirect())
            .WillOnce(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentIsHttpRedirect)
                );

    EXPECT_CALL(mockQNetworkReply, attribute(QNetworkRequest::HttpStatusCodeAttribute))
            .WillOnce(ReturnPointee(&mockQVariant));
    EXPECT_CALL(mockQVariant, toInt(nullptr))
            .WillOnce(Return(GetParam()));

    EXPECT_EQ(mockDownloadManager.isHttpRedirect(), codes.value(GetParam()));
}

INSTANTIATE_TEST_CASE_P(PossibleHttpRedirectCodes, DownloadManagerTest, ValuesIn(codes.keys()));


TEST_F(DownloadManagerTest, appendUrls)
{
    EXPECT_CALL(mockDownloadManager, appendUrls(urls))
            .Times(2)
            .WillRepeatedly(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentAppendUrls)
                );

    {
        InSequence s;
        EXPECT_CALL(mockQString, toLocal8Bit())
                .WillOnce(Return(qByteArray));
        EXPECT_CALL(mockQurl, fromEncoded(qByteArray, parsingMode))
                .WillOnce(ReturnPointee(&url));
        EXPECT_CALL(mockDownloadManager, append(_)); // WARNING : pass url instead _
        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(true));
        EXPECT_CALL(mockQTimer, singleShot(0, downloadManager, _)); // WARNING: add SIGNAL
    }
    mockDownloadManager.appendUrls(urls);

    {
        InSequence s;
        EXPECT_CALL(mockQString, toLocal8Bit())
                .WillOnce(Return(qByteArray));
        EXPECT_CALL(mockQurl, fromEncoded(qByteArray, parsingMode)) // WARNING : pass params instead _
                .WillOnce(ReturnPointee(&url));
        EXPECT_CALL(mockDownloadManager, append(_)); // WARNING : pass url instead _
        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(false));
    }

    mockDownloadManager.appendUrls(urls);
}

TEST_F(DownloadManagerTest, append)
{
    EXPECT_CALL(mockDownloadManager, append(url))
            .Times(2)
            .WillRepeatedly(
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

    {
        InSequence s;

        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(false));
        EXPECT_CALL(mockQqueue, enqueue(url));
    }

    mockDownloadManager.append(url);
}

TEST_F(DownloadManagerTest, saveFileName)
{
    EXPECT_CALL(mockDownloadManager, saveFileName(_))
            .Times(2)
            .WillRepeatedly(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentSaveFileName)
                );

    QString result;

    {
        InSequence s;
        EXPECT_CALL(mockQurl, path(QUrl::FullyDecoded))
                .WillOnce(Return(path));
        EXPECT_CALL(mockQFileInfo, setFile(path));
        EXPECT_CALL(mockQFileInfo, fileName())
                .WillOnce(ReturnRef(mockQString));
        EXPECT_CALL(mockQString, isEmpty())
                .WillOnce(Return(false));
        EXPECT_CALL(mockQFile, exists(result))
                .WillOnce(Return(false));
    }

    EXPECT_THAT(result, mockDownloadManager.saveFileName(mockQurl));

    result = "download.2";

    {
        InSequence s;
        EXPECT_CALL(mockQurl, path(QUrl::FullyDecoded))
                .WillOnce(Return(path));
        EXPECT_CALL(mockQFileInfo, setFile(path));
        EXPECT_CALL(mockQFileInfo, fileName())
                .WillOnce(ReturnRef(mockQString));
        EXPECT_CALL(mockQString, isEmpty())
                .WillOnce(Return(false));
        EXPECT_CALL(mockQFile, exists(mockQString))
                .WillOnce(Return(true));
        EXPECT_CALL(baseName, arg(0, 0, 10, QChar(' ')))
                .WillOnce(Return(QString("download.0")));
        EXPECT_CALL(mockQFile, exists(QString("download.0")))
                .WillOnce(Return(true));
        EXPECT_CALL(baseName, arg(1, 0, 10, QChar(' ')))
                .WillOnce(Return(QString("download.1")));
        EXPECT_CALL(mockQFile, exists(QString("download.1")))
                .WillOnce(Return(true));
        EXPECT_CALL(baseName, arg(2, 0, 10, QChar(' ')))
                .WillOnce(Return(result));
        EXPECT_CALL(mockQFile, exists(result))
                .WillOnce(Return(false));
    }

    EXPECT_THAT(result, mockDownloadManager.saveFileName(mockQurl));
    EXPECT_THAT(QString("download.%1"), *DownloadManager().baseName);
}

TEST_F(DownloadManagerTest, startNextDownload)
{
    QObject::connect(downloadManager, &DownloadManager::successfully,
                     modulesGroupModel, &ModulesGroupModel::extractRegistry);
    QObject::connect(downloadManager, &DownloadManager::finished,
                     &DownloadManager::downloadFinished);

    EXPECT_CALL(mockDownloadManager, startNextDownload())
            .Times(3)
            .WillRepeatedly(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentStartNextDownload)
                );

    {
        InSequence s;

        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(true));
        EXPECT_CALL(mockDownloadManager, downloadFinished());
        EXPECT_EQ(mockDownloadManager.downloadedCount, mockDownloadManager.totalCount);
        EXPECT_CALL(mockModulesGroupModel, extractRegistry());
    }

    mockDownloadManager.startNextDownload();

    {
        InSequence s;

        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(false));
        EXPECT_CALL(mockQqueue, dequeue())
                .WillOnce(ReturnPointee(&url));
        EXPECT_CALL(mockDownloadManager, saveFileName(_)) // WARNING : pass url instead _
                .WillOnce(Return(filename));
        EXPECT_CALL(mockQFile, setFileName(filename));
        EXPECT_CALL(mockQFile, fileName())
                .WillOnce(Return(filename));
        EXPECT_CALL(mockQStringList, append(filename));
        EXPECT_CALL(mockQFile, open(qFileWriteMode))
                .WillOnce(Return(true));
        EXPECT_CALL(mockQNetworkRequest, setUrl(_)); // WARNING : pass url instead _
        EXPECT_CALL(mockQNetworkAccessManager, get(_));
        // WARNING: EXPECT_CALL 3 * connect
        // WARNING: EXPECT_CALL printf
        EXPECT_CALL(mockQTime, start());
    }

    mockDownloadManager.startNextDownload();

    {
        InSequence s;

        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(false));
        EXPECT_CALL(mockQqueue, dequeue())
                .WillOnce(ReturnPointee(&url));
        EXPECT_CALL(mockDownloadManager, saveFileName(_)) // WARNING : pass url instead _
                .WillOnce(Return(filename));
        EXPECT_CALL(mockQFile, setFileName(filename));
        EXPECT_CALL(mockQFile, fileName())
                .WillOnce(Return(filename));
        EXPECT_CALL(mockQStringList, append(filename));
        EXPECT_CALL(mockQFile, open(_))
                .WillOnce(Return(false));
        EXPECT_CALL(mockDownloadManager, startNextDownload());
    }

    mockDownloadManager.startNextDownload();
}

TEST_F(DownloadManagerTest, downloadProgress)
{
    EXPECT_CALL(mockDownloadManager, downloadProgress(_, _))
            .WillOnce(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentDownloadProgress)
                );


    StrictMock<MockQString> mqArg;
    StrictMock<MockQString> mqArg2;
    QString qstring;
    {
        InSequence s;
        EXPECT_CALL(mockTextProgressBar, setStatus(bytesReceived, bytesTotal));
        EXPECT_CALL(mockQString, fromLatin1(_, -1)) // WARNING: add "%1 %2" instead _
                .WillOnce(ReturnPointee(&mqArg));
        EXPECT_CALL(mqArg, arg(_, 3, 'f', 1, _))
                .WillOnce(ReturnPointee(&mqArg2));
        EXPECT_CALL(mqArg2, arg(QString(), 0, QChar(' ')))
                .WillOnce(Return(qstring));
        EXPECT_CALL(mockTextProgressBar, setMessage(qstring));
        EXPECT_CALL(mockTextProgressBar, update());
    }

    mockDownloadManager.downloadProgress(bytesReceived, bytesTotal);
}

TEST_F(DownloadManagerTest, downloadFinished)
{
    EXPECT_CALL(mockDownloadManager, downloadFinished())
            .Times(3)
            .WillRepeatedly(
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

    // error
    mockDownloadManager.downloadFinished();

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

    // NotRedirect

    {
        InSequence s;
        EXPECT_CALL(mockTextProgressBar, clear());
        EXPECT_CALL(mockQFile, close());
        EXPECT_CALL(mockQNetworkReply, error())
                .WillOnce(Return(QNetworkReply::NetworkError::NoError));
        EXPECT_CALL(mockDownloadManager, isHttpRedirect())
                .WillOnce(Return(false));
        EXPECT_CALL(mockQNetworkReply, deleteLater());
        EXPECT_CALL(mockDownloadManager, startNextDownload());
    }

    EXPECT_EQ(mockDownloadManager.downloadedCount, mockDownloadManager.totalCount);
    mockDownloadManager.downloadFinished();
    EXPECT_EQ(mockDownloadManager.downloadedCount, 1);
}

TEST_F(DownloadManagerTest, downloadReadyRead)
{
    EXPECT_CALL(mockDownloadManager, downloadReadyRead())
            .Times(1)
            .WillOnce(
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
    EXPECT_CALL(mockDownloadManager, isHttpRedirect())
            .Times(1)
            .WillOnce(
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
    EXPECT_CALL(mockDownloadManager, reportRedirect())
            .Times(3)
            .WillRepeatedly(
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
                .WillOnce(ReturnRef(mockQurl));
        EXPECT_CALL(mockQurl, toDisplayString(QUrl::FormattingOptions(QUrl::PrettyDecoded)));
        EXPECT_CALL(mockQNetworkReply, attribute(QNetworkRequest::RedirectionTargetAttribute))
                .WillOnce(ReturnPointee(&mockQVariantTarget));
        EXPECT_CALL(mockQVariantTarget, isValid())
                .WillOnce(Return(false));
    }

    mockDownloadManager.reportRedirect();

    // TargetNotValid
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
        EXPECT_CALL(mockQurlRedirect, isRelative())
                .WillOnce(Return(false));
        EXPECT_CALL(mockQurlRedirect, toDisplayString(QUrl::FormattingOptions(QUrl::PrettyDecoded)));
    }

    mockDownloadManager.reportRedirect();

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
        EXPECT_CALL(mockQurlRedirect, isRelative())
                .WillOnce(Return(true));
        EXPECT_CALL(mockQurl, resolved(_)); // WARNING: add redirectUrl
        EXPECT_CALL(mockQurlRedirect, toDisplayString(QUrl::FormattingOptions(QUrl::PrettyDecoded)));
    }

    mockDownloadManager.reportRedirect();
}
