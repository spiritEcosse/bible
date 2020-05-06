#pragma once

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
#include "modulesmodel.h"
#include "LocaleDesc.h"
#include "gtest/gtest_prod.h"
#include <QAbstractListModel>
#include "modulesgroups.h"
#include "modulesgroupsreader.h"

#define REGISTRY "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw"
#define REGISTRY_INFO ""
#include <iostream>
#include <memory>

class ModulesGroupsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ModulesGroupsModel();
    virtual ~ModulesGroupsModel();
    static void registerMe(const std::string& moduleName);
    int rowCount(const QModelIndex& parent = {}) const override;

//    QSqlQuery* query_ = new QSqlQuery();
//    DownloadManager* manager = new DownloadManager();

    virtual void init();
//    virtual bool execLastError(const QString& query);
//    virtual void checkAvailabilityNewModules();
//    virtual QString correctTitle(const QString &name, const QString &language="", const QString &region="") const;
//    virtual QMap<QString, QString> makeGroup(const QString &name, const QString &language="", const QString &region="") const;
//    //    Q_INVOKABLE virtual void updateModules();
//    virtual void updateModules();
    virtual QVariant data(const QModelIndex &index = {}, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
//    QUrl* urlRegistryInfo = new QUrl();
//    QUrl* urlRegistry = new QUrl();

private:
    virtual void newRows(QJsonArray &downloads);
    std::vector<ModulesGroups> m_groups;
    ModulesGroupsReader m_reader;
    QString dbName = "registry_%1";
    bool update();
    void setNameDB(const QString& value);
    QString nameDB;

//    const char* registryBase64 = "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw";
//    const char* registryInfoBase64 = "";
//    ModulesModel* modulesModel = new ModulesModel();
    QFile* registry = new QFile();
//    QFile* registryVersion = new QFile();
//    QJsonParseError* qJsonParserError = new QJsonParseError();
//    QByteArray* qQByteArray = new QByteArray();
//    QJsonDocument* qJsonDocument;
//    QFile registryArchive;
//    QSettings* qSettings = new QSettings();
//    int countOldRows = 0;
//    QString tableNameString = "modules_group";
    int correctSize(const QString &str) const;
//    QString* qStringSql = new QString(
//                "CREATE TABLE IF NOT EXISTS '%1' ("
//                "   'id'        INTEGER PRIMARY KEY AUTOINCREMENT, "
//                "   'language'  CHAR(50), "
//                "   'type'      CHAR(50), "
//                "   'region'    CHAR(50) "
//                ")"
//                );
//    QString* qStringSelectSql = new QString("SELECT COUNT(*) as count FROM %1");
    friend class ModulesGroupsModelTest;
    friend class DownloadModules;
    FRIEND_TEST(ModulesGroupsModelTest, createTable);
    FRIEND_TEST(DownloadManagerTest, startNextDownload);
    FRIEND_TEST(ModulesGroupsModelTest, updateTable);
    FRIEND_TEST(ModulesGroupsModelTest, compareVersions);
    FRIEND_TEST(ModulesGroupsModelTest, updateModules);
    FRIEND_TEST(ModulesGroupsModelTest, setCountOldRows);

    friend class MockModulesGroupsModel;
    FRIEND_TEST(MockModulesGroupsModel, updateTable);

    friend class ModulesModelTest;
    FRIEND_TEST(ModulesGroupsModelTest, correctSize);

//signals:
//    void updateTableSuccess();
//    void availabilityNewModules(bool);
//    void decompressSuccess();
//    void removeRegistryFileSuccess();
//    void removeOldRowsSuccess();
//private slots:
//    virtual void updateTable();
//    virtual void compareVersions();
//    virtual void decompressRegistry();
//    void removeRegistryFile();

//public slots:
//    void removeOldRows();
};
