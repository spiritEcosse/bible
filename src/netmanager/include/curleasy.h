#ifndef CURLEASY_H
#define CURLEASY_H

#include <functional>
#include <curl/curl.h>
#include <QMap>
#include <QObject>
#include <QUrl>
#include <ctime>
#include <iostream>
#include <chrono>
#include <memory>
#include <QDebug>
#include <QSaveFile>
#include <QFile>

namespace netmanager {

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;

    using EasyHandle = std::unique_ptr<CURL, std::function<void(CURL*)>>;

    class CurlEasy : public QObject
    {
        Q_OBJECT
    private:
        std::unique_ptr<QSaveFile> downloadFile;
        QUrl m_url;
        void createEasyHandle();
        void initSaveFile();
        void setSaveFile();
        void onTransferDone();
    public:
        using DataFunction = std::function<size_t(char *buffer, size_t size)>;
        using SeekFunction = std::function<int(qint64 offset, int origin)>;

        explicit CurlEasy(QUrl&& url, bool progress = false, bool sslVerify = false, QObject *parent = nullptr) noexcept;
        virtual ~CurlEasy();
        Clock::time_point t0;

        QString getFileName();
        void perform();
        void abort();
        bool isRunning() { /*return runningOnMulti_ != nullptr;*/ }
        CURLcode result() {
            return lastResult_;
        }

        // For the list of available set options and valid parameter types consult curl_easy_setopt manual
        template<typename T> bool set(CURLoption option, T parameter) {
            return curl_easy_setopt(m_handle, option, parameter) == CURLE_OK;
        }
        bool set(CURLoption option, const QString &parameter); // Convenience override for const char* parameters
        bool set(CURLoption option, const QUrl &parameter); // Convenience override for const char* parameters
        void setReadFunction(const DataFunction &function);
        void setHeaderFunction(const DataFunction &function);
        void setSeekFunction(const SeekFunction &function);

        // For the list of available get options and valid parameter types consult curl_easy_getinfo manual
        template<typename T> bool get(CURLINFO info, T *pointer) {
            return curl_easy_getinfo(m_handle, info, pointer) == CURLE_OK;
        }
        template<typename T> T get(CURLINFO info);


        QString httpHeader(const QString &header) const;
        void setHttpHeader(const QString &header, const QString &value);
        bool hasHttpHeader(const QString &header) const;
        void removeHttpHeader(const QString &header);

        QByteArray httpHeaderRaw(const QString &header) const;
        void setHttpHeaderRaw(const QString &header, const QByteArray &encodedValue);

        CURL* handle() { return m_handle; }
//        void setPreferredMulti(CurlMulti *multi) { preferredMulti_ = multi; }

        // Safety hack: substitue QObject's deleteLater whenever possible to make sure
        // that no callbacks will be called between deleteLater and curl handle removal.
        Q_SLOT void deleteLater();

    signals:
        void aborted();
        void progress(qint64 downloadTotal, qint64 downloadNow, qint64 uploadTotal, qint64 uploadNow);
        void done(CURLcode result);

    protected:
        void removeFromMulti();
        void onCurlMessage(CURLMsg *message);
        void rebuildCurlHttpHeaders();

        static size_t staticCurlReadFunction(char *data, size_t size, size_t nitems, void *easyPtr);
        static size_t staticCurlWriteFunction(char *data, size_t size, size_t nitems, void *easyPtr);
        static size_t staticCurlHeaderFunction(char *data, size_t size, size_t nitems, void *easyPtr);
        static int staticCurlSeekFunction(void *easyPtr, curl_off_t offset, int origin);
        static int staticCurlXferInfoFunction(void *easyPtr, curl_off_t downloadTotal, curl_off_t downloadNow, curl_off_t uploadTotal, curl_off_t uploadNow);


        CURL* m_handle = nullptr;
//        CurlMulti       *preferredMulti_ = nullptr;
//        CurlMulti       *runningOnMulti_ = nullptr;
        CURLcode        lastResult_ = CURLE_OK;
        DataFunction    readFunction_;
        DataFunction    writeFunction_;
        DataFunction    headerFunction_;
        SeekFunction    seekFunction_;
        bool            m_successCommited;

        bool                        httpHeadersWereSet_ = false;
        QMap<QString, QByteArray>   httpHeaders_;
        struct curl_slist*          curlHttpHeaders_ = nullptr;

        friend class CurlMulti;
    };

    template<typename T> T CurlEasy::get(CURLINFO info)
    {
        T parameter;
        get(info, &parameter);
        return parameter;
    }
}

#endif // CURLTRANSFER_H
