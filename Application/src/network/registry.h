#ifndef DOWNLOADMODULES_H
#define DOWNLOADMODULES_H

#include <QObject>
#include "downloadmanager.h"


class Registry : public QObject
{
    Q_OBJECT
public:
    Registry();
    virtual void download();
    virtual void checkNewVersion();
private:
    DownloadManager manager;
    QFile registryArchive;
    QFile registry { "registry.json" };
signals:
    void removeRegistrySuccess();
    void decompressSuccess();
private slots:
    virtual void decompressRegistry();
    virtual void removeRegistry();
    virtual void compareVersions();
};

#endif // DOWNLOADMODULES_H
