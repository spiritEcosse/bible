#ifndef QSQLTABLEMODEL_H
#define QSQLTABLEMODEL_H

#include <QSqlRecord>
#include <QModelIndex>
#include <QSqlDatabase>
#include <QSqlQueryModel>


class QSqlTableModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    enum EditStrategy {OnFieldChange, OnRowChange, OnManualSubmit};
    QSqlDatabase db;
    virtual ~QSqlTableModel() {}
    explicit QSqlTableModel(QObject *parent = nullptr)
        : QSqlQueryModel(parent) {}

    inline virtual void setTable(const QString &) {}
    inline virtual QSqlRecord record(int) const { return QSqlRecord(); }
    inline virtual QSqlRecord record() const { return QSqlRecord(); }
    inline virtual QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const {
        Q_UNUSED(role)
        return QVariant();
    }
    inline virtual bool insertRecord(int row, const QSqlRecord &record) {
        Q_UNUSED(row)
        Q_UNUSED(record)
        return true;
    }
    inline virtual bool select() { return true; }
    inline virtual bool submitAll() { return true; }
    inline virtual QSqlDatabase& database() { return db; }
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) {
        Q_UNUSED(row)
        Q_UNUSED(count)
        Q_UNUSED(parent)
        return true;
    }
    virtual void setEditStrategy(EditStrategy strategy) {
        Q_UNUSED(strategy)
    }
    virtual QString tableName() const {
        return QString();
    }
};

#endif // QSQLTABLEMODEL_H
