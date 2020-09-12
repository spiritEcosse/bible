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

namespace TestManagerGroup
{
   class tst_ManagerGroup;
}

class ManagerRegistry : public QObject
{
    Q_OBJECT
public:
    ManagerRegistry(QObject *parent = nullptr);
    virtual ~ManagerRegistry() {}

public slots:
    virtual void download() const;

private:
    friend class TestManagerRegistry::tst_ManagerRegistry;
    friend class TestManagerGroup::tst_ManagerGroup;

    QFile registryArchive;
    QFile fileRegistryInfo;
    std::unique_ptr<ModelRegistry> m_modelRegistry;
    QFile fileRegistry { "download/registry.json" };
    std::unique_ptr<DownloadManager> m_manager;
    virtual bool hasNewRegistry(int version);
    virtual const QJsonArray getDownloads(const QJsonDocument& document) const;
    virtual int getVersion(const QJsonDocument& document) const;
    virtual void getDocument(QFile& file);

signals:
    void newRegistryAvailable(bool available, int version);
    void retrieveDataSuccess(const QJsonArray& array);
    void removeRegistrySuccess();
    void getDocumentSuccess(const QJsonDocument& document);

private slots:
    virtual void startDownload(const Registry& registry) const;
    virtual void retrieveData(const QJsonDocument& document);
    virtual void retrieveDataInfo(const QJsonDocument& document);
    virtual void extractRegistry(const QString& fileName);
    virtual void removeRegistry();
    virtual void checkNewRegistry();
};

#endif // MANAGERREGISTRY_H
