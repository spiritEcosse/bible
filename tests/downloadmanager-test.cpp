#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock_downloadmanager.h"
#include "mock_iqtimer.h"

using::testing::_;
using::testing::Mock;
using::testing::Invoke;
using::testing::Property;
using testing::internal::BuiltInDefaultValue;

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
  MockIQTimer mockIQTimer;
  const QUrl url = BuiltInDefaultValue<const QUrl>::Get();
};


TEST_F(DownloadManagerTest, append)
{
    ON_CALL(mockDownloadManager, append(url))
            .WillByDefault(Invoke(&mockDownloadManager, &MockDownloadManager::parentAppend));

    QQueue<QUrl> queue = QQueue<QUrl>{};
    downloadManager->timer = &mockIQTimer;
    EXPECT_CALL(*mockDownloadManager.timer, singleShot());
    EXPECT_EQ(queue, downloadManager->downloadQueue);
    EXPECT_EQ(0, downloadManager->totalCount);
    downloadManager->append(url);
    queue.enqueue(url);
    //singleShot
    EXPECT_EQ(queue, downloadManager->downloadQueue);
    EXPECT_EQ(1, downloadManager->totalCount);
}
