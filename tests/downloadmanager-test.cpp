#include "gtest_global.h"

#include "mock_downloadmanager.h"
#include "mock_qtimer.h"
#include "mock_qqueue.h"
#include "mock_qurl.h"


class DownloadManagerTest : public ::testing::Test
{
protected:
  DownloadManagerTest()
      : downloadManager(&mockDownloadManager)
  {}

  ~DownloadManagerTest() override {}

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
  MockDownloadManager mockDownloadManager;
  DownloadManager* downloadManager;
  MockQQueue<QUrl> mockQqueue;
  MockQTimer mockQTimer;
  MockQUrl mockQurl;

  const QUrl url = BuiltInDefaultValue<const QUrl>::Get();
  const QStringList urls = {"url1"};
};


TEST_F(DownloadManagerTest, append)
{
    ON_CALL(mockDownloadManager, append(url))
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentAppend)
                );

    mockDownloadManager.timer = &mockQTimer;
    mockDownloadManager.downloadQueue = &mockQqueue;

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

    {
        InSequence s;

        EXPECT_CALL(mockQqueue, isEmpty())
                .WillOnce(Return(false));
        EXPECT_CALL(mockQqueue, enqueue(url));
    }
    mockDownloadManager.append(url);
    EXPECT_EQ(2, mockDownloadManager.totalCount);
}

TEST_F(DownloadManagerTest, appendUrls)
{
    ON_CALL(mockDownloadManager, appendUrls(urls))
            .WillByDefault(
                    Invoke(&mockDownloadManager, &MockDownloadManager::parentAppendUrls)
                );
    mockDownloadManager.qurl = &mockQurl;
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
