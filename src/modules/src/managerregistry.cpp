#include "managerregistry.h"

#include <QFile>
#include <QByteArray>
#include <QFileInfo>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSettings>
#include <JlCompress.h>

ManagerRegistry::ManagerRegistry(QObject *parent)
    : QObject(parent), m_modelRegistry { new ModelRegistry {} }
{
    connect(&manager, &DownloadManager::readyRead, this, &ManagerRegistry::extractRegistry);
    connect(&manager, &DownloadManager::failed, this, &ManagerRegistry::download);
    connect(this, &ManagerRegistry::retrieveDataSuccess, &ManagerRegistry::removeRegistry);
    connect(this, &ManagerRegistry::retrieveDataSuccess, &*m_modelRegistry, &ModelRegistry::update);
}

void ManagerRegistry::download()
{
    try {
        auto registry = m_modelRegistry->getRegisry();
        manager.append(registry->url());
    } catch (const std::out_of_range&) {
    }
}

void ManagerRegistry::extractRegistry(const QString& fileName)
{
    registryArchive.setFileName(fileName);
    QString nameFileRegistry = QFileInfo(fileRegistry).fileName();

    if (JlCompress::getFileList(registryArchive.fileName()).contains(nameFileRegistry)) {
        JlCompress::extractFile(registryArchive.fileName(), nameFileRegistry, fileRegistry.fileName());
        retrieveData();
    }
}

void ManagerRegistry::removeRegistry()
{
    registryArchive.exists() && registryArchive.remove();
    fileRegistry.exists() && fileRegistry.remove();
    emit removeRegistrySuccess();
}

const QJsonArray ManagerRegistry::getDownloads(const QJsonDocument& document)
{
    return document.object().value("downloads").toArray();
}

void ManagerRegistry::retrieveData()
{
    QJsonParseError retrieveResult;
    QJsonDocument document;
    bool error = !fileRegistry.open(QIODevice::ReadOnly | QIODevice::Text);

    if ( !error ) {
        document = QJsonDocument::fromJson(fileRegistry.readAll(), &retrieveResult);
        fileRegistry.close();
    }

    error |= retrieveResult.error != QJsonParseError::NoError;
    const QJsonArray& array = error ? QJsonArray() : getDownloads(document);

    if (!array.isEmpty()) {
        emit retrieveDataSuccess(array);
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
    QJsonParseError retrieveResult;
    QJsonDocument document;

    if ( fileRegistryInfo.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        document = QJsonDocument::fromJson(fileRegistryInfo.readAll(), &retrieveResult);
        fileRegistryInfo.close();
    }

    bool error = retrieveResult.error != QJsonParseError::NoError;
    bool reg = !error && hasNewRegistry(getVersion(document));
    emit newRegistryAvailable(reg, error ? "Something wrong." : QString());
}

bool ManagerRegistry::hasNewRegistry(int version)
{
    QSettings qSettings;
    return qSettings.value("registryVersion").toInt() < version;
}

int ManagerRegistry::getVersion(const QJsonDocument &document)
{
    return document.object().value("version").toInt();
}
