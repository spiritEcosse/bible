#ifndef MODULESMODEL_H
#define MODULESMODEL_H

#include <QSqlTableModel>

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>

#include <qmath.h>

#include <QRegularExpression>

#include "gtest/gtest_prod.h"
#include "modules.h"
#include <iostream>

class ModulesModel : public QSqlTableModel
{
    Q_OBJECT
public:
    ModulesModel(QObject *parent = nullptr);
    virtual ~ModulesModel() {}

    QSqlQuery* query_ = new QSqlQuery();
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const;
    virtual void init();
    virtual bool createTable();
    virtual bool execLastError(const QString& query);
private:
    std::vector<Modules> m_modules;
    friend class ModulesGroupModelTest;
    friend class ModulesModelTest;
    FRIEND_TEST(ModulesModelTest, createTable);
    QString tableNameString = "modules";
    QString tableNameGroup = "modules_group";
    QString* qStringSql = new QString(
                "CREATE TABLE IF NOT EXISTS '%1' ("
                "   'id'                INTEGER PRIMARY KEY AUTOINCREMENT, "
                "   'name'              CHAR(200) NOT NULL, "
                "   'description'       TEXT, "
                "   'abbreviation'      CHAR(50), "
                "   'information'       TEXT, "
                "   'language'          CHAR(50), "
                "   'language_show'     CHAR(50), "
                "   'update'            TEXT, "
                "   'urls'              TEXT, "
                "   'comment'           TEXT, "
                "   'size'              NUMERIC NOT NULL, "
                "   'region'            TEXT, "
                "   'default_download'  NUMERIC DEFAULT 0, "
                "   'hidden'            NUMERIC DEFAULT 0, "
                "   'copyright'         TEXT, "
                "   '%2_id'             NUMERIC NOT NULL, "
                "FOREIGN KEY ('%2_id')  REFERENCES %2(id)"
                ")"
                );
};

#endif // MODULESMODEL_H
