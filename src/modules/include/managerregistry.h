#ifndef MANAGERREGISTRY_H
#define MANAGERREGISTRY_H

#include <QObject>
#include <QFile>
#include <memory>

#include "downloadmanager.h"
#include "modelregistry.h"

class QJsonParseError;

namespace modules {

    namespace tests
    {
       class tst_ManagerRegistry;
       class tst_ManagerGroup;
       class tst_ModelGroupModules;
    }

    class ManagerRegistry : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool newVersionAvailable READ newVersionAvailable)
    public:
        ManagerRegistry(QObject *parent = nullptr);
        virtual ~ManagerRegistry() {}

    public slots:
        virtual void download() const;
        virtual void checkNewVesion();

    private:
        friend class tests::tst_ManagerRegistry;
        friend class tests::tst_ManagerGroup;
        friend class tests::tst_ModelGroupModules;

        std::unique_ptr<ModelRegistry> m_modelRegistry;
        std::unique_ptr<DownloadManager> m_manager;
        bool m_newVersionAvailable = false;
        bool newVersionAvailable() const;

        QFile registryArchive;
        QFile fileRegistryInfo;
        QFile fileRegistry { "download/registry.json" };
        std::unique_ptr<Registry> m_registry;
        virtual bool hasNewRegistry(int version) const;
        virtual int getVersion(const QJsonDocument& document) const;
        virtual int getVersion() const;
        virtual void getDocument(QFile& file);
        virtual void setVersion(bool available, int version) const;
        virtual const QJsonArray getRegistries(const QJsonDocument &document) const;

    signals:
        void newRegistryAvailable(bool available, int version);
        void retrieveDataSuccess(const QJsonDocument& document);
        void removeRegistrySuccess();
        void removeInfoSuccess();
        void getDocumentSuccess(const QJsonDocument& document);
        void transformSuccess(const std::vector<Registry>& container);

    private slots:
        virtual void downloadRegistry(const Registry& registry);
        virtual void downloadInfo(const Registry& registry);
        virtual void retrieveData(const QJsonDocument& document);
        virtual void retrieveDataInfo(const QJsonDocument& document);
        virtual void extractRegistry(const QString& fileName);
        virtual void removeRegistry();
        virtual void removeInfo();
        virtual void retrieveVersion(const QString& fileName);
        virtual void transform(const QJsonDocument& document);
    };

}

#endif // MANAGERREGISTRY_H
