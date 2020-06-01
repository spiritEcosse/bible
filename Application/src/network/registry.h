#ifndef DOWNLOADMODULES_H
#define DOWNLOADMODULES_H

#include <QObject>
#include "registryinfo.h"

class Registry : public RegistryInfo
{
    Q_OBJECT
public:
    Registry();
    virtual void tryDownload();
private:
    friend class tst_Registry;
    QFile registryArchive;
    QFile file { "registry.json" };
    virtual void download(const QByteArray& registryBase64) override;
    virtual const QJsonArray getDownloads(const QJsonDocument& document);
signals:
    void decompressSuccess(const QJsonArray& document);
    void removeRegistrySuccess();
private slots:
    virtual void decompressRegistry();
    virtual void removeRegistry();
};

#endif // DOWNLOADMODULES_H
