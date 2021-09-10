#include "curlmulti.h"
#include <QDir>
#include <limits>
#include <memory>
#include <QThreadStorage>
#include <QTimer>
#include <QSocketNotifier>
#include <chrono>
#include <thread>
#include <iostream>

namespace netmanager {

    struct CurlMultiSocket
    {
        curl_socket_t socketDescriptor = CURL_SOCKET_BAD;
        QSocketNotifier *readNotifier = nullptr;
        QSocketNotifier *writeNotifier = nullptr;
        QSocketNotifier *errorNotifier = nullptr;
    };

    CurlMulti::CurlMulti(std::vector<QUrl>&& urls, bool options, QObject *parent)
        : QObject(parent)
        , m_timer(new QTimer(this))
    {
        qDebug() << "begin init CurlMulti";
        createMultiHandle();
        createTransfersFromUrls(std::move(urls));

        m_timer->setSingleShot(true);
        connect(m_timer.get(), &QTimer::timeout, this, &CurlMulti::curlMultiTimeout);

        if (options) {
            additionalOptions();
        }
    }

    CurlMulti::~CurlMulti()
    {
        removeTransfers();
    }

    void CurlMulti::additionalOptions()
    {
        curl_multi_setopt(handle_->get(), CURLMOPT_SOCKETFUNCTION, staticCurlSocketFunction);
        curl_multi_setopt(handle_->get(), CURLMOPT_SOCKETDATA, this);
        curl_multi_setopt(handle_->get(), CURLMOPT_TIMERFUNCTION, staticCurlTimerFunction);
        curl_multi_setopt(handle_->get(), CURLMOPT_TIMERDATA, this);
    }

    void CurlMulti::createMultiHandle()
    {
        handle_.reset(new MultiHandle(curl_multi_init(), curl_multi_cleanup));

        if (!handle_)
        {
            throw std::runtime_error("Failed creating CURL multi object");
        }
    }

    void CurlMulti::multiLoop()
    {
        int still_running = 0; /* keep number of running handles */

        /* we start some action by calling perform right away */
        curl_multi_perform(handle_->get(), &still_running);
        qDebug() << "curl_multi_perform";

        while (still_running) {
            struct timeval timeout = getTimeout();

            auto rc = waitIfNeeded(timeout);

            if (rc >= 0)
            {
                /* timeout or readable/writable sockets */
                qDebug() << "curl_multi_perform curl_multi_perform";
                curl_multi_perform(handle_->get(), &still_running);
            }
            /* else select error */
        }
        qDebug() << "curl_multi_perform";

    }

    void CurlMulti::perform()
    {
        multiLoop();
    }

    void CurlMulti::createTransfersFromUrls(std::vector<QUrl>&& urls)
    {
        std::for_each(urls.begin(), urls.end(), [this] (auto& url) {
            addTransfer(std::make_unique<CurlEasy>(std::move(url)));
        });
    }

    void CurlMulti::addTransfer(std::unique_ptr<CurlEasy> transfer)
    {
        curl_multi_add_handle(handle_->get(), transfer->handle());
        transfers_.push_back(std::move(transfer));
    }

    int CurlMulti::waitIfNeeded(timeval& timeout)
    {
        fd_set fdread;
        fd_set fdwrite;
        fd_set fdexcep;
        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);
        int maxfd = -1;
        /* get file descriptors from the transfers */
        auto mc = curl_multi_fdset(handle_->get(), &fdread, &fdwrite, &fdexcep, &maxfd);

        if (mc != CURLM_OK) {
            qDebug() << "curl_multi_fdset() failed, code " << mc << ".";
        }
        /* On success the value of maxfd is guaranteed to be >= -1. We call
               sleep for 100ms, which is the minimum suggested value in the
               curl_multi_fdset() doc. */
        if (maxfd == -1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        int rc = maxfd != -1 ? select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout) : 0;
        return rc;
    }

    timeval CurlMulti::getTimeout()
    {
        long curl_timeo = -1;
        /* set a suitable timeout to play around with */
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        curl_multi_timeout(handle_->get(), &curl_timeo);
        if (curl_timeo >= 0) {
            timeout.tv_sec = curl_timeo / 1000;
            if (timeout.tv_sec > 1)
                timeout.tv_sec = 1;
            else
                timeout.tv_usec = (curl_timeo % 1000) * 1000;
        }
        return timeout;
    }

    void CurlMulti::removeTransfers()
    {
        std::for_each(transfers_.begin(), transfers_.end(), [this] (auto& transfer_) {
            curl_multi_remove_handle(handle_->get(), transfer_->handle());
        });
        transfers_.clear();
    }

    int CurlMulti::curlSocketFunction(CURL *easyHandle, curl_socket_t socketDescriptor, int action, CurlMultiSocket *socket)
    {
        Q_UNUSED(easyHandle);
        if (!socket) {
            if (action == CURL_POLL_REMOVE || action == CURL_POLL_NONE)
                return 0;

            socket = new CurlMultiSocket;
            socket->socketDescriptor = socketDescriptor;
            curl_multi_assign(handle_->get(), socketDescriptor, socket);
        }

        if (action == CURL_POLL_REMOVE) {
            curl_multi_assign(handle_->get(), socketDescriptor, nullptr);

            // Note: deleteLater will NOT work here since there are
            //       situations where curl subscribes same sockect descriptor
            //       until events processing is done and actual delete happen.
            //       This causes QSocketNotifier not to register notifications again.
            if (socket->readNotifier) delete socket->readNotifier;
            if (socket->writeNotifier) delete socket->writeNotifier;
            if (socket->errorNotifier) delete socket->errorNotifier;
            delete socket;
            return 0;
        }

        if (action == CURL_POLL_IN || action == CURL_POLL_INOUT) {
            if (!socket->readNotifier) {
                socket->readNotifier = new QSocketNotifier(socket->socketDescriptor, QSocketNotifier::Read);
                connect(socket->readNotifier, &QSocketNotifier::activated, this, &CurlMulti::socketReadyRead);
            }
            socket->readNotifier->setEnabled(true);
        } else {
            if (socket->readNotifier)
                socket->readNotifier->setEnabled(false);
        }

        if (action == CURL_POLL_OUT || action == CURL_POLL_INOUT) {
            if (!socket->writeNotifier) {
                socket->writeNotifier = new QSocketNotifier(socket->socketDescriptor, QSocketNotifier::Write);
                connect(socket->writeNotifier, &QSocketNotifier::activated, this, &CurlMulti::socketReadyWrite);
            }
            socket->writeNotifier->setEnabled(true);
        } else {
            if (socket->writeNotifier)
                socket->writeNotifier->setEnabled(false);
        }

        return 0;
    }

    int CurlMulti::curlTimerFunction(int timeoutMsec)
    {
        if (timeoutMsec >= 0)
            m_timer->start(timeoutMsec);
        else
            m_timer->stop();

        return 0;
    }

    void CurlMulti::curlMultiTimeout()
        { curlSocketAction(CURL_SOCKET_TIMEOUT, 0); }

    void CurlMulti::socketReadyRead(int socketDescriptor)
        { curlSocketAction(socketDescriptor, CURL_CSELECT_IN); }

    void CurlMulti::socketReadyWrite(int socketDescriptor)
        { curlSocketAction(socketDescriptor, CURL_CSELECT_OUT); }

    void CurlMulti::socketException(int socketDescriptor)
        { curlSocketAction(socketDescriptor, CURL_CSELECT_ERR); }

    void CurlMulti::curlSocketAction(curl_socket_t socketDescriptor, int eventsBitmask)
    {
        int runningHandles;
        CURLMcode rc = curl_multi_socket_action(handle_->get(), socketDescriptor, eventsBitmask, &runningHandles);
        if (rc != 0) {
            // TODO: Handle global curl errors
        }

        int messagesLeft = 0;
        do {
            CURLMsg *message = curl_multi_info_read(handle_->get(), &messagesLeft);

            if (message == nullptr)
                break;

            if (message->easy_handle == nullptr)
                continue;

            CurlEasy *transfer = nullptr;
            curl_easy_getinfo(message->easy_handle, CURLINFO_PRIVATE, &transfer);

            if (transfer == nullptr)
                continue;

            transfer->onCurlMessage(message);
        } while (messagesLeft);
    }

    int CurlMulti::staticCurlSocketFunction(CURL *easyHandle, curl_socket_t socketDescriptor, int what, void *userp, void *sockp)
    {
        Q_UNUSED(easyHandle);
        CurlMulti *multi = static_cast<CurlMulti*>(userp);
        Q_ASSERT(multi != nullptr);

        return multi->curlSocketFunction(easyHandle, socketDescriptor, what, static_cast<CurlMultiSocket*>(sockp));
    }

    int CurlMulti::staticCurlTimerFunction(CURLM *multiHandle, long timeoutMs, void *userp)
    {
        Q_UNUSED(multiHandle);
        CurlMulti *multi = static_cast<CurlMulti*>(userp);
        Q_ASSERT(multi != nullptr);

        int intTimeoutMs;

        if (timeoutMs >= std::numeric_limits<int>::max())
            intTimeoutMs = std::numeric_limits<int>::max();
        else if (timeoutMs >= 0)
            intTimeoutMs = static_cast<int>(timeoutMs);
        else
            intTimeoutMs = -1;

        return multi->curlTimerFunction(intTimeoutMs);
    }


//    using MultiHandle = std::unique_ptr<CURLM, std::function<void(CURLM*)>>;

//    using EasyHandle = std::unique_ptr<CURL, std::function<void(CURL*)>>;

//    MultiHandle CreateMultiHandle()
//    {
//        auto curl = MultiHandle(curl_multi_init(), curl_multi_cleanup);
//        if (!curl)
//        {
//            throw std::runtime_error("Failed creating CURL multi object");
//        }
//        return curl;
//    }

//    EasyHandle CreateEasyHandle()
//    {
//        auto curl = EasyHandle(curl_easy_init(), curl_easy_cleanup);
//        if (!curl)
//        {
//            throw std::runtime_error("Failed creating CURL easy object");
//        }
//        return curl;
//    }

//    int wait_if_needed(CURLM* multi_handle, timeval& timeout)
//    {
//        fd_set fdread;
//        fd_set fdwrite;
//        fd_set fdexcep;
//        FD_ZERO(&fdread);
//        FD_ZERO(&fdwrite);
//        FD_ZERO(&fdexcep);
//        int maxfd = -1;
//        /* get file descriptors from the transfers */
//        auto mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

//        if (mc != CURLM_OK) {
//            qDebug() << "curl_multi_fdset() failed, code " << mc << ".";
//        }
//        /* On success the value of maxfd is guaranteed to be >= -1. We call
//               sleep for 100ms, which is the minimum suggested value in the
//               curl_multi_fdset() doc. */
//        if (maxfd == -1) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        }

//        int rc = maxfd != -1 ? select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout) : 0;
//        return rc;
//    }

//    timeval get_timeout(CURLM* multi_handle)
//    {
//        long curl_timeo = -1;
//        /* set a suitable timeout to play around with */
//        struct timeval timeout;
//        timeout.tv_sec = 1;
//        timeout.tv_usec = 0;

//        curl_multi_timeout(multi_handle, &curl_timeo);
//        if (curl_timeo >= 0) {
//            timeout.tv_sec = curl_timeo / 1000;
//            if (timeout.tv_sec > 1)
//                timeout.tv_sec = 1;
//            else
//                timeout.tv_usec = (curl_timeo % 1000) * 1000;
//        }
//        return timeout;
//    }

//    void multi_loop(CURLM* multi_handle)
//    {
//        int still_running = 0; /* keep number of running handles */

//        /* we start some action by calling perform right away */
//        curl_multi_perform(multi_handle, &still_running);

//        while (still_running) {
//            struct timeval timeout = get_timeout(multi_handle);

//            auto rc = wait_if_needed(multi_handle, timeout);

//            if (rc >= 0)
//            {
//                /* timeout or readable/writable sockets */
//                curl_multi_perform(multi_handle, &still_running);
//            }
//            /* else select error */
//        }
//    }

//    size_t write_to_file(void* contents, size_t size, size_t nmemb, FILE* userp)
//    {
//        return fwrite(contents, size, nmemb, userp);
//    }

//    void set_ssl(CURL* curl)
//    {
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//    }

//    void save_to_file(CURL* curl, FILE *file)
//    {
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
//        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//    }

//    int download_asynchronous(void)
//    {
//        std::vector<const char*> urls = {
//            "https://speed.hetzner.de/100MB.bin",
//    //        "https://sabnzbd.org/tests/internetspeed/50MB.bin",
//            "https://apache.org",
//            "https://www.un.org",
//        };

//        std::list<EasyHandle> handles(urls.size());
//        MultiHandle multi_handle;

//        /* init easy and multi stacks */
//        try
//        {
//            multi_handle = CreateMultiHandle();
//            std::for_each(handles.begin(), handles.end(), [](auto& handle){handle = CreateEasyHandle(); });
//        }
//        catch (const std::exception& ex)
//        {
//            qDebug() << ex.what();
//            return -1;
//        }
//        /* set options */
//        int i = 0;
//        std::vector<FILE *> list = {};

//        std::for_each(handles.begin(), handles.end(), [&i, &urls, &list](auto& handle) {
//            FILE *file = fopen(std::string("downloaded_data_" + std::to_string(i)).c_str(), "w");
//            list.push_back(file);
//            curl_easy_setopt(handle.get(), CURLOPT_URL, urls[i]);
//            set_ssl(handle.get());
//            save_to_file(handle.get(), file);
//            qDebug() << urls[i] << i;
//            i++;
//        });

//        /* add the individual transfers */
//        std::for_each(handles.begin(), handles.end(), [&multi_handle](auto& handle) {
//            curl_multi_add_handle(multi_handle.get(), handle.get()); });

//        qDebug() << "before - multi_loop";
//        multi_loop(multi_handle.get());
//        qDebug() << "after - multi_loop";

//        i = 0;
//        std::for_each(handles.begin(), handles.end(), [&multi_handle, &list, &i](auto& handle) {
//            curl_multi_remove_handle(multi_handle.get(), handle.get());
//            fclose(list[i]);
//            i++;
//        });
//    } //It was originally published on https://www.apriorit.com/















//    static std::once_flag flagSetHosts;

//    Downloader::Downloader(const QString &moduleName, QObject *parent)
//        : QObject(parent),
//          m_moduleName { std::move(moduleName) },
//          m_index { 0 },
//          m_modelHost { new modules::ModelHost {} }
//    {
//        setSaveFile();
//        setUrl();
//    }

//    Downloader::~Downloader() {}

//    const QUrl Downloader::getUrl() const
//    {
//        return m_url;
//    }

//    QNetworkReply::NetworkError Downloader::error() const
//    {
//        return m_networkReply->error();
//    }

//    void Downloader::setUrl()
//    {
//        std::call_once(flagSetHosts, [&]() {
//            m_modelHost->populateStaticObjects();
//        });

//        m_url = std::move(QString().sprintf(
//                    std::move(m_modelHost->data(m_index, modules::ModelHost::HostRoles::PathRole).toLocal8Bit().data()),
//                    std::move(m_moduleName.toLocal8Bit().data())));
//        qDebug() << m_url;
//    }

//    void Downloader::setUrlPermanent(const QUrl& url)
//    {
//        m_url = url;
//        emit urlChanged();
//    }

//    void Downloader::initSaveFile()
//    {
//        if (!m_saveFile->open(QIODevice::WriteOnly)) {
//            qDebug() << "sdsd" << m_saveFile->errorString();
////            emit error(Error::ErrorDestination, m_saveFile->errorString());
//            shutdownSaveFile();
//        }
//    }

//    void Downloader::setSaveFile()
//    {
//        m_saveFile.reset(new QSaveFile(QDir::currentPath() + "/modules/" + m_moduleName + ".zip"));
//        qDebug() << "setSaveFile" << m_saveFile->fileName();
//        initSaveFile();
//    }

//    void Downloader::onReadyRead()
//    {
////        qDebug() << "onReadyRead";
//        m_saveFile->write(m_networkReply->readAll());
//    }

//    void Downloader::start()
//    {
//        qDebug() << "start" << m_url;
////        QNetworkAccessManager manager;
////        m_networkReply = manager.get(QNetworkRequest(getUrl()));
//        connect(m_networkReply, &QNetworkReply::readyRead, this, &Downloader::onReadyRead);
//        connect(m_networkReply, &QNetworkReply::finished, this, &Downloader::onFinished);
//        //        emit started();
//    }

//    void Downloader::multiple(const std::unique_ptr<std::vector<std::tuple<QString>>> &downloaded)
//    {
//        QList<QUrl> urls { QUrl("https://sabnzbd.org/tests/internetspeed/50MB.bin")};
//        QNetworkAccessManager manager;
//        QList<QNetworkReply *> replies;
//        QEventLoop loop;
//        int total = urls.length();

//        QSaveFile m_saveFile(QDir::currentPath() + "/modules/50MB.bin");

//        if (!m_saveFile.open(QIODevice::WriteOnly)) {
//            qDebug() << "sdsd" << m_saveFile.errorString();
//        }

//        QNetworkReply *reply = manager.get(QNetworkRequest(urls[0]));
//        QObject::connect(reply, &QNetworkReply::finished, [&total, &loop](){
//            total--;
//            if(total == 0){
//                loop.quit();
//            }
//        });
//        QObject::connect(reply, &QNetworkReply::readyRead, [&m_saveFile,&reply](){
//            m_saveFile.write(reply->readAll());
//        });
//        loop.exec();
//        m_saveFile.commit();
//    }

//    void Downloader::onFinished()
//    {
//        QVariant redirectionTarget = m_networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
//        if (m_networkReply->error()) {
//            m_saveFile->cancelWriting();
////            emit error(Error::ErrorNetwork, m_networkReply->errorString());
//        } else if (!redirectionTarget.isNull()) {
//            setUrlPermanent(m_url.resolved(redirectionTarget.toUrl()));
//            return;
//        } else {
//            if(m_saveFile->commit()) {
//                shutdownSaveFile();
//                emit finished();
//            } else {
//                if(m_saveFile)
//                    m_saveFile->cancelWriting();
////                emit error(Error::ErrorDestination,"Error while writing \"" + m_saveFile->fileName() + "\"");
//            }
//        }

//        shutdownSaveFile();
//    }

//    void Downloader::shutdownSaveFile()
//    {
//        if(m_saveFile) {
//            m_saveFile->commit();
//            m_saveFile = 0;
//        }
//    }
}
