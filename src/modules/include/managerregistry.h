#ifndef MANAGERREGISTRY_H
#define MANAGERREGISTRY_H

#include <QObject>
#include <QFile>
#include <memory>

#include "downloadmanager.h"
#include "modelregistry.h"

class QJsonParseError;

namespace TestManagerRegistry
{
   class tst_ManagerRegistry;
}

class ManagerRegistry : public QObject
{
    Q_OBJECT
public:
    ManagerRegistry(QObject *parent = nullptr);
    virtual ~ManagerRegistry() {}

public slots:
    virtual void download();

private:
    friend class TestManagerRegistry::tst_ManagerRegistry;

    QFile registryArchive;
    QFile fileRegistryInfo;
    std::unique_ptr<ModelRegistry> m_modelRegistry;
    QFile fileRegistry { "download/registry.json" };
    DownloadManager manager;
    virtual bool hasNewRegistry(int version);
    virtual const QJsonArray getDownloads(const QJsonDocument& document);
    virtual int getVersion(const QJsonDocument& document);

signals:
    void newRegistryAvailable(bool, QString);
    void retrieveDataSuccess(const QJsonArray& array);
    void removeRegistrySuccess();

private slots:
    virtual void retrieveData();
    virtual void extractRegistry(const QString& fileName);
    virtual void removeRegistry();
    void checkNewRegistry();
};

#endif // MANAGERREGISTRY_H
