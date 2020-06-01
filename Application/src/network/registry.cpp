#include <QByteArray>
#include <quazip/JlCompress.h>

#include "registry.h"

const char* registryFileName { "registry.json" };

Registry::Registry() {}

void Registry::download(const QByteArray& registryBase64)
{
    connect(&manager, SIGNAL (successfully()), SLOT (decompressRegistry())); // WARNING : add connect to test
    manager.append(QUrl::fromEncoded(QByteArray::fromBase64(registryBase64)));
//    connect(this, SIGNAL (decompressSuccess()), SLOT (updateTable())); // WARNING : add connect to test
//    connect(this, SIGNAL (updateTableSuccess()), SLOT (removeOldRows())); // WARNING : add connect to test
//    connect(this, SIGNAL (updateTableSuccess()), SLOT (removeRegistry())); // WARNING : add connect to test
}

void Registry::decompressRegistry()
{
    registryArchive.setFileName(manager.fileNames->last());
    QStringList registryNames = JlCompress::extractFiles(registryArchive.fileName(), QStringList(file.fileName()), "download");
    QFileInfo fileInfo(registryNames[0]);

    if (fileInfo.fileName() == file.fileName()) {
        QJsonDocument document;
        QJsonParseError retriveResult;
        retriveData(document, retriveResult);
        bool error = retriveResult.error != QJsonParseError::NoError;

        emit decompressSuccess(getDownloads(document));
    }
}

void Registry::removeRegistry()
{
    if (registryArchive.remove() && file.remove()) {
        emit removeRegistrySuccess();
    }
}

void Registry::tryDownload()
{
//    download();
}

const QJsonArray Registry::getDownloads(const QJsonDocument& document)
{
    return document.object().value(QString("downloads")).toArray();
}

