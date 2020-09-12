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
    connect(&*m_manager, &DownloadManager::readyRead, this, &ManagerRegistry::extractRegistry);
    connect(&*m_manager, &DownloadManager::failed, this, &ManagerRegistry::download);
    connect(this, &ManagerRegistry::retrieveDataSuccess, &ManagerRegistry::removeRegistry);
    connect(this, &ManagerRegistry::retrieveDataSuccess, &*m_modelRegistry, &ModelRegistry::update);
    connect(&*m_modelRegistry, &ModelRegistry::registry, this, &ManagerRegistry::startDownload);
}

void ManagerRegistry::download() const
{
    m_modelRegistry->getRegistry();
}

void ManagerRegistry::startDownload(const Registry& registry) const
{
    m_manager->append(registry.url());
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

//void ManagerRegistry::download(const QByteArray& registryInfoBase64)
//{
//    connect(&manager, &DownloadManager::successfully, this, &RegistryInfo::newRegistry);
//    manager.append(QUrl::fromEncoded(QByteArray::fromBase64(registryInfoBase64)));
//}

void ManagerRegistry::checkNewRegistry()
{
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

bool ManagerRegistry::hasNewRegistry(int version)
{
    return QSettings().value("registryVersion").toInt() < version;
}

int ManagerRegistry::getVersion(const QJsonDocument &document) const
{
    return document.object().value("version").toInt();
}
