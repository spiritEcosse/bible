#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QTimer>
#include <QtNetwork>

#include <QObject>
#include <QTimer>

#include "TextProgressBar.h"
#include "gtest/gtest_prod.h"

class DownloadManager: public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = nullptr);
    virtual ~DownloadManager() {}

    virtual void append(const QUrl &url);
    virtual void appendUrls(const QStringList &urls);
    static QString saveFileName(const QUrl &url);
    QStringList fileNames;

signals:
    void finished();
    void successfully();

private slots:
    virtual void startNextDownload();
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    virtual void downloadFinished();
    virtual void downloadReadyRead();

private:
    virtual bool isHttpRedirect() const;
    virtual void reportRedirect();
    QTimer* timer;
    QUrl* qurl;
    QQueue<QUrl> *downloadQueue;

//    QNetworkAccessManager manager;
    friend class DownloadManagerTest;
    friend class MockDownloadManager;
    FRIEND_TEST(DownloadManagerTest, append);
    FRIEND_TEST(DownloadManagerTest, appendUrls);
    FRIEND_TEST(DownloadManagerTest, startNextDownload);
    FRIEND_TEST(MockDownloadManager, parentStartNextDownload);

    QNetworkReply *currentDownload = nullptr;
    QFile output;
    QTime downloadTime;
    TextProgressBar progressBar;

    int downloadedCount = 0;
    int totalCount = 0;
};

#endif // DOWNLOADMANAGER_H
