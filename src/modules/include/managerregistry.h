#ifndef MANAGERREGISTRY_H
#define MANAGERREGISTRY_H

#include <QObject>
#include <QFile>
#include <memory>

#include "downloadmanager.h"
#include "modelhost.h"
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
        Q_PROPERTY(bool newVersionAvailable READ newVersionAvailable NOTIFY changeNewVersionAvailable)
        Q_PROPERTY(bool checkVersionCompleted READ checkVersionCompleted NOTIFY changeCheckVersionCompleted)
    public:
        ManagerRegistry(QObject *parent = nullptr);
        virtual ~ManagerRegistry() {}
        Q_INVOKABLE virtual void checkNewVesion();

    public slots:
        virtual void download() ;
        static void registerMe();

    private:
        friend class tests::tst_ManagerRegistry;
        friend class tests::tst_ManagerGroup;
        friend class tests::tst_ModelGroupModules;
        friend class ModelGroupModules;

        std::unique_ptr<ModelRegistry> m_modelRegistry;
        std::unique_ptr<ModelHost> m_modelHost;
        std::unique_ptr<DownloadManager> m_manager;

        int index = 0;
        QFile registryArchive;
        QFile fileRegistryInfo;
        QFile fileRegistry { "download/registry.json" };
        bool m_newVersionAvailable = false;
        bool m_checkVersionCompleted = false;

        virtual bool hasNewRegistry(int version) const;
        virtual bool hasNewRegistry() const;
        virtual int getVersion(const QJsonDocument& document) const;
        virtual int getVersion() const;
        virtual void getDocument(QFile& file);
        virtual void setRegistriesOnce();
        virtual void setVersion(bool available, int version);
        void downloadFile(int role);
        virtual void tryOther(int role);
        bool newVersionAvailable() const;
        void setNewVersionAvailable();
        bool checkVersionCompleted() const;

    signals:
        void changeNewVersionAvailable();
        void changeCheckVersionCompleted();
        void newRegistryAvailable(bool available, int version);
        void retrieveDataSuccess(const QJsonDocument& document);
        void removeRegistrySuccess();
        void removeInfoSuccess();
        void getDocumentSuccess(const QJsonDocument& document);
    private slots:
        virtual void retrieveData(const QJsonDocument& document);
        virtual void retrieveDataInfo(const QJsonDocument& document);
        virtual void extractRegistry(const QString& fileName);
        virtual void removeRegistry();
        virtual void removeInfo();
        virtual void retrieveVersion(const QString& fileName);
        virtual void tryOtherUrl();
        virtual void tryOtherInfoUrl();
        virtual void setCheckVersionCompleted();
        virtual void setRegistryVersion();

    };

}

#endif // MANAGERREGISTRY_H
