#ifndef QSQLTABLEMODEL_H
#define QSQLTABLEMODEL_H

#include <QtSql/qsqlrecord.h>
#include <QtCore/QModelIndex>

//enum Qt {
//    UserRole,
//    DisplayRole
//};

class QSqlTableModel
{
public:
    enum EditStrategy {OnFieldChange, OnRowChange, OnManualSubmit};
//    explicit QSqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    virtual ~QSqlTableModel() {}
    virtual void setTable(const QString &tableName) = 0;
    virtual QSqlRecord record(int) const { return QSqlRecord(); }
    virtual QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const { return QVariant(); }
    virtual bool insertRecord(int row, const QSqlRecord &record) = 0;
    virtual bool select() = 0;
    virtual bool submitAll() = 0;
};

#endif // QSQLTABLEMODEL_H
