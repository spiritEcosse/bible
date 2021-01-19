#include <QFile>
#include <QByteArray>
#include <QFileInfo>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSettings>
#include <JlCompress.h>

#include "managerregistry.h"

namespace modules {

    ManagerRegistry::ManagerRegistry(QObject *parent)
        : QObject(parent),
          m_modelRegistry { new ModelRegistry {} },
          m_manager { new DownloadManager {} }
    {
        m_newVersionAvailable = hasNewRegistry(getVersion());
        connect(m_manager.get(), &DownloadManager::failed, m_modelRegistry.get(), &ModelRegistry::getRegistry);
    }

    void ManagerRegistry::download() const
    {
        connect(m_manager.get(), &DownloadManager::readyRead, this, &ManagerRegistry::extractRegistry);
        connect(m_modelRegistry.get(), &ModelRegistry::registry, this, &ManagerRegistry::downloadRegistry);
        connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::removeRegistry);
        connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::retrieveData);
        connect(this, &ManagerRegistry::retrieveDataSuccess, &ManagerRegistry::transform);
        connect(this, &ManagerRegistry::transformSuccess, m_modelRegistry.get(), &ModelRegistry::update);

        m_registry ? m_manager->append(m_registry->urlToQUrl()) : m_modelRegistry->getRegistry();
    }

    void ManagerRegistry::downloadRegistry(const Registry &registry)
    {
        m_registry.reset(new Registry { registry });
        m_manager->append(m_registry->urlToQUrl());
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

    const QJsonArray ManagerRegistry::getRegistries(const QJsonDocument &document) const
    {
        return document.object().value("registries").toArray();
    }

    void ManagerRegistry::transform(const QJsonDocument &document)
    {
        try {
            const QJsonArray& source = getRegistries(document);

            std::vector<Registry> target;
            std::transform(source.begin(), source.end(), std::back_inserter(target),
                           [](const QJsonValue& entry)
            {
                return Registry { entry.toObject() };
            });
            emit transformSuccess(target);
        } catch(const RegistryInvalidData& e) {}
    }

    // version

    void ManagerRegistry::removeInfo()
    {
        fileRegistryInfo.remove();
        emit removeInfoSuccess();
    }

    void ManagerRegistry::checkNewVesion()
    {
        connect(m_modelRegistry.get(), &ModelRegistry::registry, this, &ManagerRegistry::downloadInfo);
        connect(m_manager.get(), &DownloadManager::readyRead, this, &ManagerRegistry::retrieveVersion);
        connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::removeInfo);
        connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::retrieveDataInfo);
        connect(this, &ManagerRegistry::newRegistryAvailable, &ManagerRegistry::setVersion);

        m_registry ? m_manager->append(m_registry->infoUrlToQUrl()) : m_modelRegistry->getRegistry();
    }

    bool ManagerRegistry::newVersionAvailable() const
    {
        return m_newVersionAvailable;
    }

    void ManagerRegistry::downloadInfo(const Registry& registry)
    {
        m_registry.reset(new Registry { registry });
        m_manager->append(registry.infoUrlToQUrl());
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
