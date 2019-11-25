#ifndef MODULESGROUPMODEL_H
#define MODULESGROUPMODEL_H

#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QRegularExpression>

#include <QDebug>
#include <QFile>
#include <qmath.h>
#include <quazip/JlCompress.h>
#include <QLocale>

#define MODULES_SPLIT_NAME "^(.+)\\.(.+)$"

#include "DownloadManager.h"
#include "LocaleDesc.h"
#include "gtest/gtest_prod.h"

#define REGISTRY "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw"
#define REGISTRY_INFO ""
#include <iostream>

class ModulesGroupModel : public QSqlTableModel
{
    Q_OBJECT
public:
    ModulesGroupModel(QObject *parent = nullptr);
    virtual ~ModulesGroupModel() {}

    QSqlQuery* query_ = new QSqlQuery();
    DownloadManager* manager = new DownloadManager();

    virtual void init();
    virtual bool createTable(const QString &tableName);
    virtual bool execLastError(const QString& query);
    virtual void checkAvailabilityNewModules();
    virtual QString correctTitle(const QString &name, const QString &language="", const QString &region="") const;
    virtual void setCountOldRows();
    virtual void newRows(QJsonArray &downloads);
    virtual QMap<QString, QString> makeGroup(const QString &name, const QString &language="", const QString &region="") const;
//    Q_INVOKABLE virtual void updateModules();
    virtual void updateModules();
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const;

//    QUrl urlRegistry = QUrl::fromEncoded(QByteArray::fromBase64(REGISTRY));
//    QUrl urlRegistryInfo = QUrl::fromEncoded(QByteArray::fromBase64(REGISTRY_INFO));
    QUrl urlRegistry;
    QUrl urlRegistryInfo;

private:
    QFile registry;
    QFile registryArchive;
    int countOldRows = 0;
    int correctSize(const QString &str) const;

    friend class ModulesGroupModelTest;
    FRIEND_TEST(ModulesGroupModelTest, createTable);
    FRIEND_TEST(DownloadManagerTest, startNextDownload);

    friend class ModulesModelTest;
    FRIEND_TEST(ModulesGroupModelTest, correctSize);

signals:
    void updateTableSuccess();
    void availabilityNewModules(bool);
    void decompressSuccess();
    void removeRegistryFileSuccess();
    void removeOldRowsSuccess();

private slots:
    void updateTable();
    void compareVersions();
    virtual void decompressRegistry();
    void removeRegistryFile();

public slots:
    void removeOldRows();
};

#endif // MODULESGROUPMODEL_H
