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
#include "ModulesModel.h"
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
    virtual bool createTable();
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
    QUrl* urlRegistryInfo = new QUrl();
    QUrl* urlRegistry = new QUrl();

private:
    const char* registryBase64 = "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw";
    const char* registryInfoBase64 = "";
    ModulesModel* modulesModel = new ModulesModel();
    QFile* registry = new QFile();
    QFile* registryVersion = new QFile();
    QJsonParseError* qJsonParserError = new QJsonParseError();
    QByteArray* qQByteArray = new QByteArray();
    QJsonDocument* qJsonDocument;
    QFile registryArchive;
    QSettings* qSettings = new QSettings();
    int countOldRows = 0;
    QString tableNameString = "modules_group";
    int correctSize(const QString &str) const;
    QString* qStringSql = new QString(
                "CREATE TABLE IF NOT EXISTS '%1' ("
                "   'id'        INTEGER PRIMARY KEY AUTOINCREMENT, "
                "   'language'  CHAR(50), "
                "   'type'      CHAR(50), "
                "   'region'    CHAR(50) "
                ")"
                );
    QString* qStringSelectSql = new QString("SELECT COUNT(*) as count FROM %1");
    friend class ModulesGroupModelTest;
    friend class DownloadModules;
    FRIEND_TEST(ModulesGroupModelTest, createTable);
    FRIEND_TEST(DownloadManagerTest, startNextDownload);
    FRIEND_TEST(ModulesGroupModelTest, updateTable);
    FRIEND_TEST(ModulesGroupModelTest, compareVersions);
    FRIEND_TEST(ModulesGroupModelTest, updateModules);
    FRIEND_TEST(ModulesGroupModelTest, setCountOldRows);

    friend class MockModulesGroupModel;
    FRIEND_TEST(MockModulesGroupModel, updateTable);

    friend class ModulesModelTest;
    FRIEND_TEST(ModulesGroupModelTest, correctSize);

signals:
    void updateTableSuccess();
    void availabilityNewModules(bool);
    void decompressSuccess();
    void removeRegistryFileSuccess();
    void removeOldRowsSuccess();
private slots:
    virtual void updateTable();
    virtual void compareVersions();
    virtual void decompressRegistry();
    void removeRegistryFile();

public slots:
    void removeOldRows();
};

#endif // MODULESGROUPMODEL_H
