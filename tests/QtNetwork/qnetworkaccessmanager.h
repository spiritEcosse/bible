#ifndef QNETWORKACCESSMANAGER_H
#define QNETWORKACCESSMANAGER_H

#include <QObject>
#include <QNetworkRequest>

class QNetworkReply;


class QNetworkAccessManager
{
public:
    enum Operation {
        HeadOperation = 1,
        GetOperation,
        PutOperation,
        PostOperation,
        DeleteOperation,
        CustomOperation,

        UnknownOperation = 0
    };

    explicit QNetworkAccessManager(QObject *parent = nullptr);
    ~QNetworkAccessManager();

    QNetworkReply *get(const QNetworkRequest &request);
};

#endif // QNETWORKACCESSMANAGER_H
