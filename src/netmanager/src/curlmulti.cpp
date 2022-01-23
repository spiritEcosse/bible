#include "curlmulti.h"
#include <QDir>
#include <memory>
#include <QSocketNotifier>
#include <iostream>

namespace netmanager {

    CurlMulti::CurlMulti(std::vector<QString>&& urls, QObject *parent)
        : QObject(parent)
    {
        createMultiHandle();
        createTransfersFromUrls(std::move(urls));
        connect(this, &CurlMulti::done, &CurlMulti::onTransferDone);
        curl_multi_setopt(m_handle, CURLMOPT_MAXCONNECTS, (long)10);
    }

    CurlMulti::~CurlMulti()
    {
        removeTransfers();

        if (m_handle) {
            curl_multi_cleanup(m_handle);
            qDebug() << "curl_multi_cleanup - removeTransfers";
        }
    }

    void CurlMulti::createMultiHandle()
    {
        m_handle = curl_multi_init();

        if (!m_handle)
        {
            throw std::runtime_error("Failed creating CURL multi object");
        }
    }

    void CurlMulti::multiLoop()
    {
        int still_running = 0; /* keep number of running handles */

        do {
            CURLMcode mc = curl_multi_perform(m_handle, &still_running);

            if(still_running)
              /* wait for activity, timeout or "nothing" */
              mc = curl_multi_poll(m_handle, NULL, 0, 100, NULL);

            if(mc)
              break;
        } while(still_running);
        emit done();
    }

    void CurlMulti::onTransferDone()
    {
        int messagesLeft = 0;
        do {
            CURLMsg *message = curl_multi_info_read(m_handle, &messagesLeft);

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
        emit downloaded();
    }

    void CurlMulti::perform()
    {
        multiLoop();
    }

    Downloaded CurlMulti::getSuccessfullyDownloaded()
    {
        Downloaded data;
        std::for_each(transfers_.begin(), transfers_.end(), [&data](auto& transfer_) {
            if (transfer_->m_successCommited) {
                data.push_back(std::make_tuple(transfer_->getFileName().split(".zip").first()));
            }
        });
        return data;
    }

    Downloaded CurlMulti::getFailedDownloaded()
    {
        Downloaded data;
        std::for_each(transfers_.begin(), transfers_.end(), [&data](auto& transfer_) {
            if (!transfer_->m_successCommited) {
                data.push_back(std::make_tuple(transfer_->getFileName().split(".zip").first()));
            }
        });
        return data;
    }

    void CurlMulti::createTransfersFromUrls(std::vector<QString>&& urls)
    {
        std::for_each(urls.begin(), urls.end(), [this] (auto& url) {
            addTransfer(std::make_unique<CurlEasy>(std::move(url)));
        });
    }

    void CurlMulti::addTransfer(std::unique_ptr<CurlEasy> transfer)
    {
        curl_multi_add_handle(m_handle, transfer->handle());
        transfers_.push_back(std::move(transfer));
    }

    void CurlMulti::removeTransfers()
    {
        std::for_each(transfers_.begin(), transfers_.end(), [this] (auto& transfer_) {
            curl_multi_remove_handle(m_handle, transfer_->handle());
        });
    }

}
