#ifndef QSQLTABLEMODEL_H
#define QSQLTABLEMODEL_H

#include <QSqlRecord>
#include <QModelIndex>
#include <QSqlDatabase>
#include <QObject>


class QSqlTableModel : public QObject
{
    Q_OBJECT
public:
    QSqlDatabase db;
    virtual ~QSqlTableModel() {}
    explicit QSqlTableModel(QObject *parent = nullptr) {}

    virtual inline void setTable(const QString &) {}
    virtual inline QSqlRecord record(int) const { return QSqlRecord(); }
    virtual inline QSqlRecord record() const { return QSqlRecord(); }
    virtual inline QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const {
        Q_UNUSED(role)
        return QVariant();
    }
    virtual inline bool insertRecord(int row, const QSqlRecord &record) {
        Q_UNUSED(row)
        Q_UNUSED(record)
        return true;
    }
    virtual inline bool select() { return true; }
    virtual inline bool submitAll() { return true; }
    virtual inline QSqlDatabase& database() { return db; }
};

#endif // QSQLTABLEMODEL_H
