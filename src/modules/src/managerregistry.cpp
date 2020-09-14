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
    connect(&*m_manager, &DownloadManager::failed, &*m_modelRegistry, &ModelRegistry::getRegistry);
}

void ManagerRegistry::download() const
{
    connect(&*m_manager, &DownloadManager::readyRead, this, &ManagerRegistry::extractRegistry);

    connect(&*m_modelRegistry, &ModelRegistry::registry, this, &ManagerRegistry::downloadRegistry);

    connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::removeRegistry);
    connect(this, &ManagerRegistry::retrieveDataSuccess, &*m_modelRegistry, &ModelRegistry::update);

    m_manager->append(m_registry->url());
}

void ManagerRegistry::downloadRegistry(const Registry &registry)
{
    m_registry.reset(new Registry { registry });
    download();
}

void ManagerRegistry::extractRegistry(const QString& fileName)
{
    registryArchive.setFileName(fileName);
    QString nameFileRegistry = QFileInfo(fileRegistry).fileName();

    if (JlCompress::getFileList(registryArchive.fileName()).contains(nameFileRegistry)) {
        JlCompress::extractFile(registryArchive.fileName(), nameFileRegistry, fileRegistry.fileName());
        connect(this, &ManagerRegistry::getDocumentSuccess, this, &ManagerRegistry::retrieveData);
        getDocument(fileRegistry);
    }
}

void ManagerRegistry::removeRegistry()
{
    registryArchive.exists() && registryArchive.remove();
    fileRegistry.exists() && fileRegistry.remove();
    emit removeRegistrySuccess();
}

void ManagerRegistry::removeRegistryInfo()
{
    emit removeRegistryInfoSuccess();
}

const QJsonArray ManagerRegistry::getDownloads(const QJsonDocument& document) const
{
    return document.object().value("downloads").toArray();
}

void ManagerRegistry::retrieveData(const QJsonDocument& document)
{
    const QJsonArray& array = getDownloads(document);

    if (!array.isEmpty() && hasNewRegistry(getVersion(document))) {
        emit retrieveDataSuccess(array);
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

// registryVersion

void ManagerRegistry::checkNewVesion() const
{
    connect(&*m_manager, &DownloadManager::readyRead, this, &ManagerRegistry::retrieveVersion);
    connect(this, &ManagerRegistry::getDocumentSuccess, &ManagerRegistry::removeRegistryInfo);
    connect(&*m_modelRegistry, &ModelRegistry::registry, this, &ManagerRegistry::downloadInfo);

    m_modelRegistry->getRegistry();
}

void ManagerRegistry::downloadInfo(const Registry& registry)
{
    m_registry.reset(new Registry { registry });
    m_manager->append(registry.infoUrl());
}

void ManagerRegistry::retrieveVersion(const QString& fileName)
{
    fileRegistryInfo.setFileName(fileName);
    connect(this, &ManagerRegistry::getDocumentSuccess, this, &ManagerRegistry::retrieveDataInfo);
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
