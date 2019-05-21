#ifndef MODULESMODEL_H
#define MODULESMODEL_H

#include <QJsonValue>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextDocumentFragment>
#include <QRegularExpression>
#include <qmath.h>
#include <QDebug>
#include <quazip/JlCompress.h>
#include <QLocale>
#define MODULES_SPLIT_NAME "^(.+)\\.(.+)$"

#include "DownloadManager.h"
#include "LocaleDesc.h"

#define REGISTRY "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw"
#define REGISTRY_INFO ""

class ModulesModel : public QSqlTableModel
{
    Q_OBJECT
public:
    ModulesModel(QObject *parent = 0);
    ~ModulesModel();

    DownloadManager manager;
    QVariant data(const QModelIndex &index, int role) const override;
    QString tableName() const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE void updateModules();
    QUrl urlRegistry = QUrl::fromEncoded(QByteArray::fromBase64(REGISTRY));
    QUrl urlRegistryInfo = QUrl::fromEncoded(QByteArray::fromBase64(REGISTRY_INFO));
    void checkAvailabilityNewModules();
    int correctSize(const QString &str) const;
    QString section(const QString &name, const QString &language="", const QString &region="") const;
    void setCountOldRows();

private:
    void newRows(QJsonArray &downloads);
    QFile registry;
    QFile registryArchive;
    int countOldRows = 0;

signals:
    void updateTableSuccess();
    void availabilityNewModules(bool);
    void decompressSuccess();
    void removeRegistryFileSuccess();
    void removeOldRowsSuccess();

private slots:
    void updateTable();
    void compareVersions();
    void decompressRegistry();
    void removeRegistryFile();

public slots:
    void removeOldRows();
};

#endif // MODULESMODEL_H
