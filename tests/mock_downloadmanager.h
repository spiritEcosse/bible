#ifndef MOCK_DOWNLOADMANAGER_H
#define MOCK_DOWNLOADMANAGER_H

#include <gmock/gmock.h>

#include "../src/DownloadManager.h"

class MockDownloadManager : public DownloadManager
{
public:
    MOCK_METHOD1(appendUrls, void(const QStringList &urls));
    MOCK_METHOD1(append, void(const QUrl &url));
    MOCK_METHOD0(startNextDownload, void());
    MOCK_METHOD2(downloadProgress, void(qint64 bytesReceived, qint64 bytesTotal));
    MOCK_METHOD0(downloadFinished, void());
    MOCK_METHOD0(downloadReadyRead, void());
    MOCK_CONST_METHOD0(isHttpRedirect, bool());
    MOCK_METHOD0(reportRedirect, void());

    void parentAppend(const QUrl &url)
    {
      return DownloadManager::append(url);
    }
    void parentAppendUrls(const QStringList &urls)
    {
        return DownloadManager::appendUrls(urls);
    }
    void parentStartNextDownload()
    {
        return DownloadManager::startNextDownload();
    }
};

#endif // MOCK_DOWNLOADMANAGER_H
