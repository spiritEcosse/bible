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
    virtual QString saveFileName(const QUrl &url);
    QStringList* fileNames = new QStringList();

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
    QUrl* qurl = new QUrl();
    QQueue<QUrl> *downloadQueue = new QQueue<QUrl>();
    QFileInfo* qFileInfo = new QFileInfo();
    QNetworkRequest* request = new QNetworkRequest();
    QTextStream* qTextStream = new QTextStream();

    QNetworkAccessManager* manager = new QNetworkAccessManager();
    friend class DownloadManagerTest;
    friend class MockDownloadManager;
    FRIEND_TEST(DownloadManagerTest, append);
    FRIEND_TEST(DownloadManagerTest, appendUrls);
    FRIEND_TEST(DownloadManagerTest, startNextDownload);
    FRIEND_TEST(MockDownloadManager, parentStartNextDownload);
    FRIEND_TEST(DownloadManagerTest, saveFileName);

    QNetworkReply *currentDownload = nullptr;
    QFile* output = new QFile();
    QTime* downloadTime = new QTime();
    TextProgressBar* progressBar = new TextProgressBar();

    int downloadedCount = 0;
    int totalCount = 0;
};

#endif // DOWNLOADMANAGER_H
