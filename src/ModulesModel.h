#ifndef MODULESMODEL_H
#define MODULESMODEL_H

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

#include <qmath.h>

#include <QRegularExpression>

#include "gtest/gtest_prod.h"

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
    virtual bool createTable(const QString &tableName, const QString &relatedTable);
    virtual bool execLastError(const QString& query);

private:
    friend class ModulesGroupModelTest;
    FRIEND_TEST(ModulesModelTest, createTable);
};


#endif // MODULESMODEL_H
