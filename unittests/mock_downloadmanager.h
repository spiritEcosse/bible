#ifndef MOCK_DOWNLOADMANAGER_H
#define MOCK_DOWNLOADMANAGER_H

#include <gmock/gmock.h>

#include "../../../src/DownloadManager.h"

class MockDownloadManager : public DownloadManager
{
public:
    MOCK_METHOD1(appendUrls, void(QList<QString*> &urls));
    MOCK_METHOD1(append, void(const QUrl &url));
    MOCK_METHOD0(startNextDownload, void());
    MOCK_METHOD2(downloadProgress, void(qint64 bytesReceived, qint64 bytesTotal));
    MOCK_METHOD0(downloadFinished, void());
    MOCK_METHOD0(downloadReadyRead, void());
    MOCK_CONST_METHOD0(isHttpRedirect, bool());
    MOCK_METHOD0(reportRedirect, void());
    MOCK_METHOD1(saveFileName, QString(const QUrl&));

    void parentAppend(const QUrl &url)
    {
        return DownloadManager::append(url);
    }
    void parentAppendUrls(QList<QString*> &urls)
    {
        return DownloadManager::appendUrls(urls);
    }
    void parentStartNextDownload()
    {
        return DownloadManager::startNextDownload();
    }
    void parentDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
    {
        return DownloadManager::downloadProgress(bytesReceived, bytesTotal);
    }
    void parentDownloadFinished()
    {
        return DownloadManager::downloadFinished();
    }
    void parentDownloadReadyRead()
    {
        return DownloadManager::downloadReadyRead();
    }
    bool parentIsHttpRedirect()
    {
        return DownloadManager::isHttpRedirect();
    }
    void parentReportRedirect()
    {
        return DownloadManager::reportRedirect();
    }
    QString parentSaveFileName(const QUrl &url)
    {
        return DownloadManager::saveFileName(url);
    }
};

#endif // MOCK_DOWNLOADMANAGER_H
