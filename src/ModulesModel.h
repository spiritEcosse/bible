#ifndef MODULESMODEL_H
#define MODULESMODEL_H

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

#include <qmath.h>

#include <QRegularExpression>

#include "gtest/gtest_prod.h"

#include <iostream>

template <class QSqlDatabase>
class ModulesModel : public QSqlTableModel
{
public:
    ModulesModel(QSqlDatabase &db, QObject *parent = nullptr);
    explicit ModulesModel() {}
    virtual ~ModulesModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const;
    virtual void init();

private:
    friend class ModulesModelTest;
    FRIEND_TEST(ModulesModelTest, correctSize);
    FRIEND_TEST(ModulesModelTest, createTable);

    QSqlDatabase *db;
    int correctSize(const QString &str) const;
    virtual bool createTable(const QString &tableName, const QString &relatedTable);
};


#endif // MODULESMODEL_H
