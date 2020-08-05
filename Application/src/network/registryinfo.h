#ifndef REGISTRYINFO_H
#define REGISTRYINFO_H

#include <QObject>
#include "downloadmanager.h"

class RegistryInfo : public QObject
{
    Q_OBJECT
public:
    RegistryInfo();
    virtual void download(const QByteArray& registryInfoBase64);
private:
    friend class tst_RegistryInfo;
    virtual bool hasNewRegistry(int version);
protected:
    QFile file;
//    DownloadManager manager;
    virtual int getVersion(QJsonDocument& document);
    virtual void retriveData(QJsonDocument& document, QJsonParseError& error);
signals:
    void newRegistryAvailable(bool, QString);
private slots:
    void newRegistry();
};

#endif // REGISTRYINFO_H
