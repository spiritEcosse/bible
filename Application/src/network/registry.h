#ifndef DOWNLOADMODULES_H
#define DOWNLOADMODULES_H

#include <QObject>
#include "downloadmanager.h"


class Registry : public QObject
{
    Q_OBJECT
public:
    Registry();
    virtual void download(const QByteArray& registryBase64);
    virtual void checkNewVersion();
private:
    friend class tst_Registry;
    DownloadManager manager;
    QFile registryArchive;
    QFile registry { "registry.json" };
signals:
    void removeRegistrySuccess();
    void decompressSuccess();
private slots:
    virtual void decompressRegistry();
    virtual void compareVersions();
    virtual void removeRegistry();
};

#endif // DOWNLOADMODULES_H
