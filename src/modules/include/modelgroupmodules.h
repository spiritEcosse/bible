#ifndef MODELGROUPMODULES_H
#define MODELGROUPMODULES_H

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
#include <QLocale>
#include <QObject>

#define MODULES_SPLIT_NAME "^(.+)\\.(.+)$"

#include "modelmodule.h"
//#include "LocaleDesc.h"
#include "gtest/gtest_prod.h"
//#include "modulesgroups.h"
//#include "modulesgroupsreader.h"

#define REGISTRY "aHR0cDovL215YmlibGUuaW50ZXJiaWJsaWEub3JnL3JlZ2lzdHJ5X3Rlc3Quemlw"
#define REGISTRY_INFO ""
#include <iostream>
#include <memory>
#include "managergroup.h"
#include "modelupdate.h"


namespace modules {

    namespace tests
    {
       class tst_ModelGroupModules;
    }

    class ModelGroupModules : public ModelUpdate<GroupModules>
    {
        Q_OBJECT
    public:
        ModelGroupModules();
        virtual ~ModelGroupModules();
        static void registerMe(const std::string& moduleName);
        virtual QVariant data(const QModelIndex &index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        friend class tests::tst_ModelGroupModules;

        std::unique_ptr<ManagerGroup> m_managerGroup;
    };

}

#endif // MODELGROUPMODULES_H
