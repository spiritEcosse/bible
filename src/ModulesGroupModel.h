#ifndef MODULESGROUPMODEL_H
#define MODULESGROUPMODEL_H

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QRegularExpression>

#include <QFile>
#include <qmath.h>
#include <quazip/JlCompress.h>
#include <QLocale>

#define MODULES_SPLIT_NAME "^(.+)\\.(.+)$"

#include "DownloadManager.h"
#include "LocaleDesc.h"

#define REGISTRY "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw"
#define REGISTRY_INFO ""


class SignalsSlots : public QObject
{
    Q_OBJECT
public:
    SignalsSlots(QObject *parent = nullptr) {}
    virtual ~SignalsSlots() {}
signals:
    virtual void updateTableSuccess() = 0;
    virtual void availabilityNewModules(bool) = 0;
    virtual void decompressSuccess() = 0;
    virtual void removeRegistryFileSuccess() = 0;
    virtual void removeOldRowsSuccess() = 0;

private slots:
    virtual void updateTable() = 0;
    virtual void compareVersions() = 0;
    virtual void decompressRegistry() = 0;
    virtual void removeRegistryFile() = 0;

public slots:
    virtual void removeOldRows() = 0;
};

template <class QSqlDatabase, class QSqlQuery>
class ModulesGroupModel : public SignalsSlots, public QSqlTableModel
{
public:
    ModulesGroupModel(QObject *parent = 0);
    ~ModulesGroupModel();

    DownloadManager manager;
    QVariant data(const QModelIndex &index, int role) const override;
    virtual QString tableName() const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE virtual void updateModules();
    QUrl urlRegistry = QUrl::fromEncoded(QByteArray::fromBase64(REGISTRY));
    QUrl urlRegistryInfo = QUrl::fromEncoded(QByteArray::fromBase64(REGISTRY_INFO));
    virtual void checkAvailabilityNewModules();
    virtual QString correctTitle(const QString &name, const QString &language="", const QString &region="") const;
    QMap<QString, QString> makeGroup(const QString &name, const QString &language="", const QString &region="") const;
    virtual void setCountOldRows();
    virtual void newRows(QJsonArray &downloads);

private:
    QFile registry;
    QFile registryArchive;
    int countOldRows = 0;
};

#endif // MODULESGROUPMODEL_H
