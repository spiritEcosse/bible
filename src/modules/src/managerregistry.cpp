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

ManagerRegistry::ManagerRegistry(QObject *parent)
    : QObject(parent),
      m_modelRegistry { new ModelRegistry {} },
      m_manager { new DownloadManager {} }
{
    connect(m_manager.get(), &DownloadManager::failed, m_modelRegistry.get(), &ModelRegistry::getRegistry);
}

void ManagerRegistry::download() const
{
    connect(m_manager.get(), &DownloadManager::readyRead, this, &ManagerRegistry::extractRegistry);
    connect(m_modelRegistry.get(), &ModelRegistry::registry, this, &ManagerRegistry::downloadRegistry);
    connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::removeRegistry);
    connect(this, &ManagerRegistry::getDocumentSuccess, this, &ManagerRegistry::retrieveData);
    connect(this, &ManagerRegistry::retrieveDataSuccess, m_modelRegistry.get(), &ModelRegistry::update);

    m_registry ? m_manager->append(m_registry->url()) : m_modelRegistry->getRegistry();
}

void ManagerRegistry::downloadRegistry(const Registry &registry)
{
    m_registry.reset(new Registry { registry });
    m_manager->append(m_registry->url());
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

// version

void ManagerRegistry::removeInfo()
{
    fileRegistryInfo.remove();
    emit removeInfoSuccess();
}

void ManagerRegistry::checkNewVesion()
{
    int version = getVersion();

    if (hasNewRegistry(version)) {
        emit newRegistryAvailable(true, version);
    } else {
        connect(m_modelRegistry.get(), &ModelRegistry::registry, this, &ManagerRegistry::downloadInfo);
        connect(m_manager.get(), &DownloadManager::readyRead, this, &ManagerRegistry::retrieveVersion);
        connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::removeInfo);
        connect(this, &ManagerRegistry::getDocumentSuccess, this, &ManagerRegistry::retrieveDataInfo);
        connect(this, &ManagerRegistry::newRegistryAvailable, this, &ManagerRegistry::setVersion);

        m_registry ? m_manager->append(m_registry->infoUrl()) : m_modelRegistry->getRegistry();
    }
}

void ManagerRegistry::downloadInfo(const Registry& registry)
{
    m_registry.reset(new Registry { registry });
    m_manager->append(registry.infoUrl());
}

void ManagerRegistry::retrieveVersion(const QString& fileName)
{
    fileRegistryInfo.setFileName(fileName);
    getDocument(fileRegistryInfo);
}

void ManagerRegistry::retrieveDataInfo(const QJsonDocument &document)
{
    int version = getVersion(document);

    if (version) {
        emit newRegistryAvailable(hasNewRegistry(version), version);
    }
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
