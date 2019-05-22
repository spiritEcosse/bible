#ifndef MODULESMODEL_H
#define MODULESMODEL_H

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

class ModulesModel : public QSqlTableModel
{
    Q_OBJECT
public:
    ModulesModel(QObject *parent = 0);
    ~ModulesModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QString tableName() const;
    QHash<int, QByteArray> roleNames() const;
};

#endif // MODULESMODEL_H
