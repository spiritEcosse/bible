#include "downloadmanager.h"

#include <QDebug>
#include <QTextStream>

#include <cstdio>

using namespace std;

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent), downloadedCount(0), totalCount(0)
{
}

void DownloadManager::appendUrls(QList<QString*> &urls)
{
    for (QString* urlAsString : urls) {
        append(qurl->fromEncoded(urlAsString->toLocal8Bit()));
    }

    if (downloadQueue->isEmpty()) {
        timer->singleShot(0, this, SIGNAL(finished()));
    }
}

void DownloadManager::append(const QUrl &url)
{
    if (downloadQueue->isEmpty()) {
        timer->singleShot(0, this, &DownloadManager::startNextDownload);
    }

    downloadQueue->enqueue(url);
    ++totalCount;
}


QString DownloadManager::saveFileName(const QUrl &url)
{
    QString path = url.path();
    qFileInfo->setFile(path);
    QString basename = QString("download/%1").arg(qFileInfo->fileName());

    if (basename.isEmpty()) {
        basename = "download/download";
    }

//    for (int index = 0; output->exists(basename); index++) {
//        basename = QString("%1.%2").arg(basename, index);
//    }






    return basename;
}

void DownloadManager::startNextDownload()
{
    if (downloadQueue->isEmpty()) {
        printf("%d/%d files downloaded successfully\n", downloadedCount, totalCount); //WARNING: needed fprintf ?
        emit finished();

        if (downloadedCount == totalCount) {
            emit successfully();
        }
    } else {
        const QUrl url = downloadQueue->dequeue();

        QString filename = QString("%1").arg(saveFileName(url));
        output->setFileName(filename);
        fileNames->append(output->fileName());

        if (!output->open(QFile::WriteOnly)) {
            fprintf(stderr, "Problem opening save file '%s' for download '%s': %s\n", //WARNING: needed fprintf ?
                    qPrintable(filename), url.toEncoded().constData(),
                    qPrintable(output->errorString()));

            startNextDownload();
        } else {
            request->setUrl(url);
            currentDownload = manager.get(*request);
            connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
                    SLOT(downloadProgress(qint64,qint64)));
            connect(currentDownload, SIGNAL(finished()),
                    SLOT(downloadFinished()));
            connect(currentDownload, SIGNAL(readyRead()),
                    SLOT(downloadReadyRead()));

//              prepare the output
            printf("Downloading %s...\n", url.toEncoded().constData()); //WARNING: needed fprintf ?
            downloadTime->start();
        }
    }
}

void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    progressBar->setStatus(bytesReceived, bytesTotal);

    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTime->elapsed();
    QString unit;
    //start WARNING: check this
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }
    //end WARNING: check this
    progressBar->setMessage(qString->fromLatin1("%1 %2").arg(speed, 3, 'f', 1).arg(unit));
    progressBar->update();
}

void DownloadManager::downloadFinished()
{
    progressBar->clear();
    output->close();

    if (currentDownload->error()) {
        // download failed
//        fprintf(stderr, "Failed: %s\n", qPrintable(currentDownload->errorString())); //WARNING: needed fprintf ?
        output->remove();
    } else {
        // let's check if it was actually a redirect
        if (isHttpRedirect()) {
            reportRedirect();
            output->remove();
        } else {
//            printf("Succeeded (saved to %s)\n", qPrintable(output->fileName())); //WARNING: needed fprintf ?
            ++downloadedCount; // WARNING: check this
        }
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    output->write(currentDownload->readAll());
}

bool DownloadManager::isHttpRedirect() const
{
    int statusCode = currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::reportRedirect()
{
    int statusCode = currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QUrl requestUrl = currentDownload->request().url();
    *qTextStream << "Request: " << requestUrl.toDisplayString() // WARNING: add mock QTextStream
                << " was redirected with code: " << statusCode
                << '\n';

    QVariant target = currentDownload->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (target.isValid()) {
        QUrl redirectUrl = target.toUrl();

        if (redirectUrl.isRelative()) {
            redirectUrl = requestUrl.resolved(redirectUrl);
        }
        *qTextStream << "Redirected to: " << redirectUrl.toDisplayString()
                            << '\n';
    }
}
