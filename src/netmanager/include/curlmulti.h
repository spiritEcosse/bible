#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <curl/curl.h>
#include <QObject>
#include "curleasy.h"

class QTimer;

namespace netmanager {

    struct CurlMultiSocket;

    using MultiHandle = std::unique_ptr<CURLM, std::function<void(CURLM*)>>;

    class CurlMulti : public QObject
    {
        Q_OBJECT
    public:
        explicit CurlMulti(std::vector<QUrl>&& urls, bool options = false, QObject *parent = nullptr);
        virtual ~CurlMulti();

        void createTransfersFromUrls(std::vector<QUrl>&& urls);
        void addTransfer(std::unique_ptr<CurlEasy> transfer);
        void perform();

    protected slots:
        void curlMultiTimeout();
        void socketReadyRead(int socketDescriptor);
        void socketReadyWrite(int socketDescriptor);
        void socketException(int socketDescriptor);

    protected:
        void curlSocketAction(curl_socket_t socketDescriptor, int eventsBitmask);
        int curlTimerFunction(int timeoutMsec);
        int curlSocketFunction(CURL *easyHandle, curl_socket_t socketDescriptor, int action, CurlMultiSocket *socket);
        static int staticCurlTimerFunction(CURLM *multiHandle, long timeoutMs, void *userp);
        static int staticCurlSocketFunction(CURL *easyHandle, curl_socket_t socketDescriptor, int what, void *userp, void *sockp);

        std::unique_ptr<QTimer> m_timer = nullptr;
        std::unique_ptr<MultiHandle> handle_ = nullptr;
        std::vector<std::unique_ptr<CurlEasy>> transfers_;
    private:
        void removeTransfers();
        void multiLoop();
        void createMultiHandle();
        int waitIfNeeded(timeval& timeout);
        timeval getTimeout();
        void additionalOptions();
    };

//    class Downloader : public QObject
//    {
//        Q_OBJECT
//    public:
//        enum Error
//        {
//            ErrorDestination,
//            ErrorNetwork
//        };
//        Q_ENUM(Error)

//        Downloader(const QString &moduleName, QObject *parent = 0);
//        ~Downloader();
//        const QUrl getUrl() const;
//        QNetworkReply::NetworkError error() const;
//        void start();
//        QNetworkReply* m_networkReply;
//        static void multiple(const std::unique_ptr<std::vector<std::tuple<QString>>> &downloaded);
//    Q_SIGNALS:
//        void saveFileChanged();
//        void finished();
//        void finishedR();
//        void started();
//        void urlChanged();
//    public slots:
//        void onFinished();
//        void onReadyRead();
////        void error(int errorCode, QString errorString);
//    private:
//        void setUrlPermanent(const QUrl& url);
//        void setSaveFile();
//        void initSaveFile();

//        QString m_moduleName;
//        bool m_running;
//        unsigned int m_index;
//        std::unique_ptr<QSaveFile> m_saveFile;
//        QUrl m_url;
//        void shutdownSaveFile();

//        std::unique_ptr<modules::ModelHost> m_modelHost;
//    private slots:
//        void setUrl();
//    };
}

#endif // DOWNLOADER_H
