#ifndef QSQLTABLEMODEL_H
#define QSQLTABLEMODEL_H

#include <QSqlRecord>
#include <QModelIndex>
#include <QSqlDatabase>

class QSqlTableModel
{
public:
    QSqlDatabase db;
    virtual ~QSqlTableModel() {}
    virtual void setTable(const QString &) = 0;
    virtual QSqlRecord record(int) const { return QSqlRecord(); }
    virtual QSqlRecord record() const { return QSqlRecord(); }
    virtual QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const {
        Q_UNUSED(role);
        return QVariant();
    }
    virtual bool insertRecord(int row, const QSqlRecord &record) = 0;
    virtual bool select() = 0;
    virtual bool submitAll() = 0;
    virtual QSqlDatabase& database() {
        return db;
    }
};

#endif // QSQLTABLEMODEL_H
