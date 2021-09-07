#include "downloader.h"
#include <QDir>
#include <mutex>
#include <QTimer>
#include <QtNetwork>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
//#include <curl/curl.h>

//static const char *urls[] = {
//  "https://www.microsoft.com",
//  "https://opensource.org",
//  "https://www.google.com",
//  "https://www.yahoo.com",
//  "https://www.ibm.com",
//  "https://www.mysql.com",
//  "https://www.oracle.com",
//  "https://www.ripe.net",
//  "https://www.iana.org",
//  "https://www.amazon.com",
//  "https://www.netcraft.com",
//  "https://www.heise.de",
//  "https://www.chip.de",
//  "https://www.ca.com",
//  "https://www.cnet.com",
//  "https://www.mozilla.org",
//  "https://www.cnn.com",
//  "https://www.wikipedia.org",
//  "https://www.dell.com",
//  "https://www.hp.com",
//  "https://www.cert.org",
//  "https://www.mit.edu",
//  "https://www.nist.gov",
//  "https://www.ebay.com",
//  "https://www.playstation.com",
//  "https://www.uefa.com",
//  "https://www.ieee.org",
//  "https://www.apple.com",
//  "https://www.symantec.com",
//  "https://www.zdnet.com",
//  "https://www.fujitsu.com/global/",
//  "https://www.supermicro.com",
//  "https://www.hotmail.com",
//  "https://www.ietf.org",
//  "https://www.bbc.co.uk",
//  "https://news.google.com",
//  "https://www.foxnews.com",
//  "https://www.msn.com",
//  "https://www.wired.com",
//  "https://www.sky.com",
//  "https://www.usatoday.com",
//  "https://www.cbs.com",
//  "https://www.nbc.com/",
//  "https://slashdot.org",
//  "https://www.informationweek.com",
//  "https://apache.org",
//  "https://www.un.org",
//};

//#define MAX_PARALLEL 10 /* number of simultaneous transfers */
//#define NUM_URLS sizeof(urls)/sizeof(char *)

//static size_t write_cb(char *data, size_t n, size_t l, void *userp)
//{
//  /* take care of the data here, ignored in this example */
//  (void)data;
//  (void)userp;
//  return n*l;
//}

//static void add_transfer(CURLM *cm, int i)
//{
//  CURL *eh = curl_easy_init();
//  curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_cb);
//  curl_easy_setopt(eh, CURLOPT_URL, urls[i]);
//  curl_easy_setopt(eh, CURLOPT_PRIVATE, urls[i]);
//  curl_multi_add_handle(cm, eh);
//}

//int multi(void)
//{
//  CURLM *cm;
//  CURLMsg *msg;
//  unsigned int transfers = 0;
//  int msgs_left = -1;
//  int still_alive = 1;

//  curl_global_init(CURL_GLOBAL_ALL);
//  cm = curl_multi_init();

//  /* Limit the amount of simultaneous connections curl should allow: */
//  curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)MAX_PARALLEL);

//  for(transfers = 0; transfers < MAX_PARALLEL; transfers++)
//    add_transfer(cm, transfers);

//  do {
//    curl_multi_perform(cm, &still_alive);

//    while((msg = curl_multi_info_read(cm, &msgs_left))) {
//      if(msg->msg == CURLMSG_DONE) {
//        char *url;
//        CURL *e = msg->easy_handle;
//        curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
//        fprintf(stderr, "R: %d - %s <%s>\n",
//                msg->data.result, curl_easy_strerror(msg->data.result), url);
//        curl_multi_remove_handle(cm, e);
//        curl_easy_cleanup(e);
//      }
//      else {
//        fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
//      }
//      if(transfers < NUM_URLS)
//        add_transfer(cm, transfers++);
//    }
//    if(still_alive)
//      curl_multi_wait(cm, NULL, 0, 1000, NULL);

//  } while(still_alive || (transfers < NUM_URLS));

//  curl_multi_cleanup(cm);
//  curl_global_cleanup();

//  return EXIT_SUCCESS;
//}

namespace netmanager {

    static std::once_flag flagSetHosts;

    Downloader::Downloader(const QString &moduleName, QObject *parent)
        : QObject(parent),
          m_moduleName { std::move(moduleName) },
          m_index { 0 },
          m_modelHost { new modules::ModelHost {} }
    {
        setSaveFile();
        setUrl();
    }

    Downloader::~Downloader() {}

    const QUrl Downloader::getUrl() const
    {
        return m_url;
    }

    QNetworkReply::NetworkError Downloader::error() const
    {
        return m_networkReply->error();
    }

    void Downloader::setUrl()
    {
        std::call_once(flagSetHosts, [&]() {
            m_modelHost->populateStaticObjects();
        });

        m_url = std::move(QString().sprintf(
                    std::move(m_modelHost->data(m_index, modules::ModelHost::HostRoles::PathRole).toLocal8Bit().data()),
                    std::move(m_moduleName.toLocal8Bit().data())));
        qDebug() << m_url;
    }

    void Downloader::setUrlPermanent(const QUrl& url)
    {
        m_url = url;
        emit urlChanged();
    }

    void Downloader::initSaveFile()
    {
        if (!m_saveFile->open(QIODevice::WriteOnly)) {
            qDebug() << "sdsd" << m_saveFile->errorString();
//            emit error(Error::ErrorDestination, m_saveFile->errorString());
            shutdownSaveFile();
        }
    }

    void Downloader::setSaveFile()
    {
        m_saveFile.reset(new QSaveFile(QDir::currentPath() + "/modules/" + m_moduleName + ".zip"));
        qDebug() << "setSaveFile" << m_saveFile->fileName();
        initSaveFile();
    }

    void Downloader::onReadyRead()
    {
//        qDebug() << "onReadyRead";
        m_saveFile->write(m_networkReply->readAll());
    }

    void Downloader::start()
    {
        qDebug() << "start" << m_url;
//        QNetworkAccessManager manager;
//        m_networkReply = manager.get(QNetworkRequest(getUrl()));
        connect(m_networkReply, &QNetworkReply::readyRead, this, &Downloader::onReadyRead);
        connect(m_networkReply, &QNetworkReply::finished, this, &Downloader::onFinished);
        //        emit started();
    }

    void Downloader::multiple(const std::unique_ptr<std::vector<std::tuple<QString>>> &downloaded)
    {
        QList<QUrl> urls { QUrl("https://sabnzbd.org/tests/internetspeed/50MB.bin")};
        QNetworkAccessManager manager;
        QList<QNetworkReply *> replies;
        QEventLoop loop;
        int total = urls.length();

        QSaveFile m_saveFile(QDir::currentPath() + "/modules/50MB.bin");

        if (!m_saveFile.open(QIODevice::WriteOnly)) {
            qDebug() << "sdsd" << m_saveFile.errorString();
        }

        QNetworkReply *reply = manager.get(QNetworkRequest(urls[0]));
        QObject::connect(reply, &QNetworkReply::finished, [&total, &loop](){
            total--;
            if(total == 0){
                loop.quit();
            }
        });
        QObject::connect(reply, &QNetworkReply::readyRead, [&m_saveFile,&reply](){
            m_saveFile.write(reply->readAll());
        });
        loop.exec();
        m_saveFile.commit();
    }

    void Downloader::onFinished()
    {
        QVariant redirectionTarget = m_networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if (m_networkReply->error()) {
            m_saveFile->cancelWriting();
//            emit error(Error::ErrorNetwork, m_networkReply->errorString());
        } else if (!redirectionTarget.isNull()) {
            setUrlPermanent(m_url.resolved(redirectionTarget.toUrl()));
            return;
        } else {
            if(m_saveFile->commit()) {
                shutdownSaveFile();
                emit finished();
            } else {
                if(m_saveFile)
                    m_saveFile->cancelWriting();
//                emit error(Error::ErrorDestination,"Error while writing \"" + m_saveFile->fileName() + "\"");
            }
        }

        shutdownSaveFile();
    }

    void Downloader::shutdownSaveFile()
    {
        if(m_saveFile) {
            m_saveFile->commit();
            m_saveFile = 0;
        }
    }
}
