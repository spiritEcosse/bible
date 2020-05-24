#include <QByteArray>
#include <quazip/JlCompress.h>

#include "registry.h"

const char* registryBase64 { "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw" };
const char* registryInfoBase64 { "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u" };
const char* registryFileName { "registry.json" };

Registry::Registry() {}

void Registry::download()
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
    QString registryName = JlCompress::extractFile(registryArchive.fileName(), registry.fileName());
    QFileInfo fileInfo(registryName);

    if (fileInfo.fileName() == registry.fileName()) {
        emit decompressSuccess();
    }
}

void Registry::removeRegistry()
{
    if (registryArchive.remove()) {
        emit removeRegistrySuccess();
    }
}

void Registry::checkNewVersion()
{
    manager.append(QUrl::fromEncoded(QByteArray::fromBase64(registryInfoBase64)));
//    connect(manager, SIGNAL (successfully()), this, SLOT (compareVersions()));
}

void Registry::compareVersions()
{
//    registryVersion->setFileName(manager->fileNames->last());

//    if ( !registryVersion->open(QIODevice::ReadOnly | QIODevice::Text) )
//        return ;

//    QJsonDocument document = qJsonDocument->fromJson(registryVersion->readAll(), qJsonParserError);
//    registryVersion->close();

//    if ( qJsonParserError->error != QJsonParseError::NoError ) // WARNING : test this
//        return;

//    int version = document.object().value("version").toInt();
//    bool newModules = version > qSettings->value("modulesVersion").toInt();

//    if (newModules) {
//        qSettings->setValue("modulesVersion", version);
//    }

//    emit availabilityNewModules(newModules);
}
