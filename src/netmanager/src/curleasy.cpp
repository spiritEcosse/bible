#include "curleasy.h"
#include <QDir>

namespace netmanager {

    CurlEasy::CurlEasy(QUrl&& url, bool progress, bool sslVerify, QObject *parent)
    noexcept
        : QObject(parent),
          m_url { std::move(url) }
    {
        createEasyHandle();

//        set(CURLOPT_MAXAGE_CONN, 1L);

        set(CURLOPT_PRIVATE, this);
        set(CURLOPT_XFERINFOFUNCTION, staticCurlXferInfoFunction);
        set(CURLOPT_XFERINFODATA, this);

        set(CURLOPT_URL, m_url);
        set(CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
        set(CURLOPT_FAILONERROR, 1L); // Do not return CURL_OK in case valid server responses reporting errors.
        set(CURLOPT_NOPROGRESS, progress ? 0L : 1L);
        set(CURLOPT_VERBOSE, 1L);
//        set(CURLOPT_FORBID_REUSE, 1L);

        auto ssl = sslVerify ? 1L : 0L;
        set(CURLOPT_SSL_VERIFYPEER, ssl);
        set(CURLOPT_SSL_VERIFYHOST, ssl);

        set(CURLOPT_WRITEFUNCTION, staticCurlWriteFunction);
        set(CURLOPT_WRITEDATA, this);

        setSaveFile();
        connect(this, &CurlEasy::done, &CurlEasy::onTransferDone);
    }

    void CurlEasy::initSaveFile()
    {
        if (!downloadFile->open(QIODevice::WriteOnly)) {
            qDebug() << downloadFile->errorString();
//            emit error(Error::ErrorDestination, m_saveFile->errorString());
//            shutdownSaveFile();
        }
    }

    void CurlEasy::setSaveFile()
    {
        downloadFile.reset(new QSaveFile(QDir::currentPath() + "/modules/" + m_url.toString().split("/").last()));
        qDebug() << m_url << downloadFile->fileName();
        initSaveFile();
    }

    QString CurlEasy::getFileName()
    {
        QFileInfo fileInfo(downloadFile->fileName());
        return fileInfo.fileName();
    }

    void CurlEasy::createEasyHandle()
    {
        m_handle = curl_easy_init();

        if (!m_handle)
        {
            throw std::runtime_error("Failed creating CURL easy object");
        }
    }

    CurlEasy::~CurlEasy()
    {
//        removeFromMulti();

        if (m_handle) {
            qDebug() << "curl_easy_cleanup >>> curl_easy_cleanup";
            curl_easy_cleanup(m_handle);
        }

//        if (curlHttpHeaders_) {
//            curl_slist_free_all(curlHttpHeaders_);
//            curlHttpHeaders_ = nullptr;
//        }
    }

    void CurlEasy::deleteLater()
    {
        removeFromMulti();
        QObject::deleteLater();
    }

    void CurlEasy::perform()
    {
        if (isRunning())
            return;

        t0 = Clock::now();

        rebuildCurlHttpHeaders();

//        if (preferredMulti_)
//            runningOnMulti_ = preferredMulti_;
//        else
//            runningOnMulti_ = CurlMulti::threadInstance();

//        runningOnMulti_->addTransfer(this);
    }

    void CurlEasy::abort()
    {
        if (!isRunning())
            return;

        removeFromMulti();

        emit aborted();
    }

    void CurlEasy::removeFromMulti()
    {
//        if (runningOnMulti_ != nullptr) {
//            runningOnMulti_->removeTransfer(this);
//            runningOnMulti_ = nullptr;
//        }
    }

    void CurlEasy::onCurlMessage(CURLMsg *message)
    {
        if (message->msg == CURLMSG_DONE) {
            lastResult_ = message->data.result;
            emit done(lastResult_);
        }
    }

    void CurlEasy::onTransferDone()
    {
        m_successCommited = result() == CURLE_OK;

        if (m_successCommited) {
            downloadFile->commit();
        } else {
            downloadFile->cancelWriting();
        }
    }

    void CurlEasy::rebuildCurlHttpHeaders()
    {
        if (!httpHeadersWereSet_)
            return;

        if (curlHttpHeaders_) {
            curl_slist_free_all(curlHttpHeaders_);
            curlHttpHeaders_ = nullptr;
        }

        for (auto it = httpHeaders_.begin(); it != httpHeaders_.end(); ++it) {
            const QString &header = it.key();
            const QByteArray &value = it.value();

            QByteArray headerString = header.toUtf8();
            headerString += ": ";
            headerString += value;
            headerString.append(char(0));

            curlHttpHeaders_ = curl_slist_append(curlHttpHeaders_, headerString.constData());
        }

        set(CURLOPT_HTTPHEADER, curlHttpHeaders_);
    }

    void CurlEasy::setReadFunction(const CurlEasy::DataFunction &function)
    {
        readFunction_ = function;
        if (readFunction_) {
            set(CURLOPT_READFUNCTION, staticCurlReadFunction);
            set(CURLOPT_READDATA, this);
        } else {
            set(CURLOPT_READFUNCTION, nullptr);
            set(CURLOPT_READDATA, nullptr);
        }
    }

    void CurlEasy::setHeaderFunction(const CurlEasy::DataFunction &function)
    {
        headerFunction_ = function;
        if (headerFunction_) {
            set(CURLOPT_HEADERFUNCTION, staticCurlHeaderFunction);
            set(CURLOPT_HEADERDATA, this);
        } else {
            set(CURLOPT_HEADERFUNCTION, nullptr);
            set(CURLOPT_HEADERDATA, nullptr);
        }
    }

    void CurlEasy::setSeekFunction(const CurlEasy::SeekFunction &function)
    {
        seekFunction_ = function;
        if (seekFunction_) {
            set(CURLOPT_SEEKFUNCTION, staticCurlSeekFunction);
            set(CURLOPT_SEEKDATA, this);
        } else {
            set(CURLOPT_SEEKFUNCTION, nullptr);
            set(CURLOPT_SEEKDATA, nullptr);
        }
    }

    size_t CurlEasy::staticCurlWriteFunction(char *data, size_t size, size_t nitems, void *easyPtr)
    {
        CurlEasy *easy = static_cast<CurlEasy*>(easyPtr);
        qint64 bytesWritten = easy->downloadFile->write(data, size*nitems);
        return static_cast<size_t>(bytesWritten);
    }

    size_t CurlEasy::staticCurlHeaderFunction(char *data, size_t size, size_t nitems, void *easyPtr)
    {
        CurlEasy *easy = static_cast<CurlEasy*>(easyPtr);
        Q_ASSERT(easy != nullptr);

        if (easy->headerFunction_)
            return easy->headerFunction_(data, size*nitems);
        else
            return  size*nitems;
    }

    int CurlEasy::staticCurlSeekFunction(void *easyPtr, curl_off_t offset, int origin)
    {
        CurlEasy *easy = static_cast<CurlEasy*>(easyPtr);
        Q_ASSERT(easy != nullptr);

        if (easy->seekFunction_)
            return easy->seekFunction_(static_cast<qint64>(offset), origin);
        else
            return CURL_SEEKFUNC_CANTSEEK;
    }

    size_t CurlEasy::staticCurlReadFunction(char *buffer, size_t size, size_t nitems, void *easyPtr)
    {
        CurlEasy *transfer = static_cast<CurlEasy*>(easyPtr);
        Q_ASSERT(transfer != nullptr);

        if (transfer->readFunction_)
            return transfer->readFunction_(buffer, size*nitems);
        else
            return size*nitems;
    }

    int CurlEasy::staticCurlXferInfoFunction(void *easyPtr, curl_off_t downloadTotal, curl_off_t downloadNow, curl_off_t uploadTotal, curl_off_t uploadNow)
    {
        CurlEasy *transfer = static_cast<CurlEasy*>(easyPtr);
        Q_ASSERT(transfer != nullptr);

        emit transfer->progress(static_cast<qint64>(downloadTotal), static_cast<qint64>(downloadNow),
                                static_cast<qint64>(uploadTotal), static_cast<qint64>(uploadNow));

        return 0;
    }

    void CurlEasy::removeHttpHeader(const QString &header)
    {
        httpHeaders_.remove(header);
        httpHeadersWereSet_ = true;
    }

    QByteArray CurlEasy::httpHeaderRaw(const QString &header) const
    {
        return httpHeaders_[header];
    }

    void CurlEasy::setHttpHeaderRaw(const QString &header, const QByteArray &encodedValue)
    {
        httpHeaders_[header] = encodedValue;
        httpHeadersWereSet_ = true;
    }

    bool CurlEasy::set(CURLoption option, const QString &parameter)
        { return set(option, parameter.toUtf8().constData()); }

    bool CurlEasy::set(CURLoption option, const QUrl &parameter)
        { return set(option, parameter.toEncoded().constData()); }

    void CurlEasy::setHttpHeader(const QString &header, const QString &value)
        { setHttpHeaderRaw(header, QUrl::toPercentEncoding(value)); }

    QString CurlEasy::httpHeader(const QString &header) const
        { return QUrl::fromPercentEncoding(httpHeaders_[header]); }

    bool CurlEasy::hasHttpHeader(const QString &header) const
        { return httpHeaders_.contains(header); }
}
