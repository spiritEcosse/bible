#include "registryinfo.h"

RegistryInfo::RegistryInfo()
{

}

void RegistryInfo::download(const QByteArray& registryInfoBase64)
{
    connect(&manager, &DownloadManager::successfully, this, &RegistryInfo::newRegistry);
    manager.append(QUrl::fromEncoded(QByteArray::fromBase64(registryInfoBase64)));
}

void RegistryInfo::retriveData(QJsonDocument& document, QJsonParseError& error)
{
    file.setFileName(manager.fileNames->last());

    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
        return;

    document = std::move(QJsonDocument::fromJson(file.readAll(), &error));
    file.close();
}

void RegistryInfo::newRegistry()
{
    QJsonParseError retriveResult;
    QJsonDocument document;
    retriveData(document, retriveResult);
    bool reg = retriveResult.error == QJsonParseError::NoError && hasNewRegistry(getVersion(document));
    emit newRegistryAvailable(reg, retriveResult.errorString());
}

bool RegistryInfo::hasNewRegistry(int version)
{
    QSettings qSettings;
    return qSettings.value("registryVersion").toInt() < version;
}

int RegistryInfo::getVersion(QJsonDocument &document)
{
    return document.object().value("version").toInt();
}
