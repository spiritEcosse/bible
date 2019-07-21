//#ifndef MODULESGROUPMODEL_H
//#define MODULESGROUPMODEL_H

//#include <QSqlRecord>
//#include <QSqlQuery>
//#include <QSqlError>
//#include <QSqlTableModel>

//#include <QJsonValue>
//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QJsonArray>

//#include <QRegularExpression>

//#include <QFile>
//#include <qmath.h>
//#include <quazip/JlCompress.h>
//#include <QLocale>

//#define MODULES_SPLIT_NAME "^(.+)\\.(.+)$"

//#include "DownloadManager.h"
//#include "LocaleDesc.h"

//#define REGISTRY "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw"
//#define REGISTRY_INFO ""

//template <class QSqlDatabase, class QSqlQuery>
//class ModulesGroupModel : public QSqlTableModel
//{
//public:
//    ModulesGroupModel(QSqlDatabase &db, QObject *parent = nullptr);
//    explicit ModulesGroupModel() {}
//    virtual ~ModulesGroupModel();

//    QSqlQuery* query_;
//    DownloadManager* manager;

//    virtual void init();
//    virtual bool createTable(const QString &tableName);
//    virtual QSqlQuery& query() const;
//    virtual bool execLastError(const QString& query);
//    virtual void checkAvailabilityNewModules();
//    virtual QString correctTitle(const QString &name, const QString &language="", const QString &region="") const;
//    virtual void setCountOldRows();
//    virtual void newRows(QJsonArray &downloads);
//    virtual QMap<QString, QString> makeGroup(const QString &name, const QString &language="", const QString &region="") const;
////    Q_INVOKABLE virtual void updateModules();
//    virtual void updateModules();
//    virtual QVariant data(const QModelIndex &index, int role) const override;
//    virtual QHash<int, QByteArray> roleNames() const;

//    QUrl urlRegistry = QUrl::fromEncoded(QByteArray::fromBase64(REGISTRY));
//    QUrl urlRegistryInfo = QUrl::fromEncoded(QByteArray::fromBase64(REGISTRY_INFO));

//private:
//    QSqlDatabase *db_;
//    QFile registry;
//    QFile registryArchive;
//    int countOldRows = 0;

//signals:
//    void updateTableSuccess();
//    void availabilityNewModules(bool);
//    void decompressSuccess();
//    void removeRegistryFileSuccess();
//    void removeOldRowsSuccess();

//private slots:
//    void updateTable();
//    void compareVersions();
//    void decompressRegistry();
//    void removeRegistryFile();

//public slots:
//    void removeOldRows();
//};

//#endif // MODULESGROUPMODEL_H
