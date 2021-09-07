#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include "modelhost.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSaveFile>

namespace netmanager {

    class Downloader : public QObject
    {
        Q_OBJECT
    public:
        enum Error
        {
            ErrorDestination,
            ErrorNetwork
        };
        Q_ENUM(Error)

        Downloader(const QString &moduleName, QObject *parent = 0);
        ~Downloader();
        const QUrl getUrl() const;
        QNetworkReply::NetworkError error() const;
        void start();
        QNetworkReply* m_networkReply;
        static void multiple(const std::unique_ptr<std::vector<std::tuple<QString>>> &downloaded);
    Q_SIGNALS:
        void saveFileChanged();
        void finished();
        void finishedR();
        void started();
        void urlChanged();
    public slots:
        void onFinished();
        void onReadyRead();
//        void error(int errorCode, QString errorString);
    private:
        void setUrlPermanent(const QUrl& url);
        void setSaveFile();
        void initSaveFile();

        QString m_moduleName;
        bool m_running;
        unsigned int m_index;
        std::unique_ptr<QSaveFile> m_saveFile;
        QUrl m_url;
        void shutdownSaveFile();

        std::unique_ptr<modules::ModelHost> m_modelHost;
    private slots:
        void setUrl();
    };
}

#endif // DOWNLOADER_H
