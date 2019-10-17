#include "gtest_global.h"

#include "mock_downloadmanager.h"
#include "mock_qtimer.h"
#include "mock_qqueue.h"


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

  const QUrl url = BuiltInDefaultValue<const QUrl>::Get();
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
}
