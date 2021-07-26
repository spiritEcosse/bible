#include <QFile>
#include <QByteArray>
#include <QFileInfo>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSettings>
#include <JlCompress.h>
#include <mutex>

#include <invaliddata.h>
#include "managerregistry.h"

namespace modules {

    static std::once_flag flagSetRegistries;

    ManagerRegistry::ManagerRegistry(QObject *parent)
        : QObject(parent),
          m_modelRegistry { new ModelRegistry {} },
          m_modelHost { new ModelHost {} },
          m_manager { new DownloadManager {} }
    {
    }

    void ManagerRegistry::download()
    {
        connect(m_manager.get(), &DownloadManager::failed, this, &ManagerRegistry::tryOtherUrl);
        connect(m_manager.get(), &DownloadManager::readyRead, this, &ManagerRegistry::extractRegistry);
        connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::removeRegistry);
        connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::retrieveData);
        connect(this, &ManagerRegistry::retrieveDataSuccess, m_modelRegistry.get(), &ModelRegistry::transform);
        connect(this, &ManagerRegistry::retrieveDataSuccess, m_modelHost.get(), &ModelHost::transform);

        downloadFile(ModelRegistry::RegistryRoles::UrlRole);
    }

    void ManagerRegistry::downloadFile(int role)
    {
        m_manager->append(std::move(m_modelRegistry->data(index, role)));
    }

    void ManagerRegistry::extractRegistry(const QString& fileName)
    {
        registryArchive.setFileName(fileName);
        QString nameFileRegistry = QFileInfo(fileRegistry).fileName();

        if (JlCompress::getFileList(registryArchive.fileName()).contains(nameFileRegistry)) {
            JlCompress::extractFile(registryArchive.fileName(), nameFileRegistry, fileRegistry.fileName());
            getDocument(fileRegistry);
        }
    }

    void ManagerRegistry::removeRegistry()
    {
        registryArchive.remove();
        fileRegistry.remove();
        emit removeRegistrySuccess();
    }

    void ManagerRegistry::retrieveData(const QJsonDocument& document)
    {
        if (hasNewRegistry(getVersion(document))) {
            emit retrieveDataSuccess(document);
        }
    }

    void ManagerRegistry::getDocument(QFile& file)
    {
        if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
            QJsonParseError retrieveResult;
            const QJsonDocument& document = QJsonDocument::fromJson(file.readAll(), &retrieveResult);
            file.close();

            if (retrieveResult.error == QJsonParseError::NoError) {
                emit getDocumentSuccess(document);
            }
        }
    }

    void ManagerRegistry::setRegistriesOnce()
    {
        std::call_once(flagSetRegistries, [&]() {
            size_t count = m_modelRegistry->m_db->count();
            if (count && m_modelRegistry->setRegistries()) {
                index = count >= 2 ? 1 : 0;
            }
        });
    }

    void ManagerRegistry::tryOther(int role)
    {
        index++;
        if (index >= static_cast<int>(m_modelRegistry->m_objects.size()))
        {
            index = 0;
            setRegistriesOnce();
        }
        if (index == 0) {
            emit m_modelRegistry->error("An error occured, please try in time.");
        } else {
            downloadFile(role);
        }
    }

    void ManagerRegistry::tryOtherUrl()
    {
        tryOther(ModelRegistry::RegistryRoles::UrlRole);
    }

    // version

    void ManagerRegistry::removeInfo()
    {
        fileRegistryInfo.remove();
        emit removeInfoSuccess();
    }

    void ManagerRegistry::checkNewVesion()
    {
        connect(m_manager.get(), &DownloadManager::failed, this, &ManagerRegistry::tryOtherInfoUrl);
        connect(m_manager.get(), &DownloadManager::readyRead, this, &ManagerRegistry::retrieveVersion);
        connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::removeInfo);
        connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::retrieveDataInfo);
        connect(this, &ManagerRegistry::newRegistryAvailable, &ManagerRegistry::setVersion);

        downloadFile(ModelRegistry::RegistryRoles::InfoUrlRole);
    }

    void ManagerRegistry::tryOtherInfoUrl()
    {
        tryOther(ModelRegistry::RegistryRoles::InfoUrlRole);
    }

    void ManagerRegistry::retrieveVersion(const QString& fileName)
    {
        fileRegistryInfo.setFileName(fileName);
        getDocument(fileRegistryInfo);
    }

    void ManagerRegistry::retrieveDataInfo(const QJsonDocument &document)
    {
        int version = getVersion(document);
        emit newRegistryAvailable(hasNewRegistry(version), version);
    }

    bool ManagerRegistry::hasNewRegistry(int version) const
    {
        return QSettings().value("registryVersion").toInt() < version;
    }

    bool ManagerRegistry::hasNewRegistry() const
    {
        return QSettings().value("registryVersion").toInt() < getVersion();
    }

    int ManagerRegistry::getVersion(const QJsonDocument &document) const
    {
        return document.object().value("version").toInt();
    }

    int ManagerRegistry::getVersion() const
    {
        return QSettings().value("cacheRegistryVersion").toInt();
    }

    void ManagerRegistry::setVersion(bool available, int version) const
    {
        if (available) {
            QSettings().setValue("cacheRegistryVersion", version);
        }
    }

}
