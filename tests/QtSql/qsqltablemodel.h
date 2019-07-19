#ifndef QSQLTABLEMODEL_H
#define QSQLTABLEMODEL_H

#include <QtSql/qtsqlglobal.h>
//#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquerymodel.h>

class QSqlTableModel : public QSqlQueryModel
{
public:
    explicit QSqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    enum EditStrategy {OnFieldChange, OnRowChange, OnManualSubmit};
    virtual ~QSqlTableModel();

    virtual void setTable(const QString &tableName);
//    QString tableName() const;

//    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QSqlRecord record() const;
    QSqlRecord record(int row) const;
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;
//    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
//#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
//    bool clearItemData(const QModelIndex &index) override;
//#endif

//    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

//    bool isDirty() const;
//    bool isDirty(const QModelIndex &index) const;

//    void clear() override;

//    virtual void setEditStrategy(EditStrategy strategy);
//    EditStrategy editStrategy() const;

//    QSqlIndex primaryKey() const;
//    QSqlDatabase database() const;
//    int fieldIndex(const QString &fieldName) const;

//    void sort(int column, Qt::SortOrder order) override;
//    virtual void setSort(int column, Qt::SortOrder order);

//    QString filter() const;
//    virtual void setFilter(const QString &filter);

//    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

//    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool insertRecord(int row, const QSqlRecord &record);
//    bool setRecord(int row, const QSqlRecord &record);

//    virtual void revertRow(int row);

//public Q_SLOTS:
    virtual bool select();
//    virtual bool selectRow(int row);

//    bool submit() override;
//    void revert() override;

    bool submitAll();
//    void revertAll();

//Q_SIGNALS:
//    void primeInsert(int row, QSqlRecord &record);

//    void beforeInsert(QSqlRecord &record);
//    void beforeUpdate(int row, QSqlRecord &record);
//    void beforeDelete(int row);

//protected:
//    QSqlTableModel(QSqlTableModelPrivate &dd, QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

//    virtual bool updateRowInTable(int row, const QSqlRecord &values);
//    virtual bool insertRowIntoTable(const QSqlRecord &values);
//    virtual bool deleteRowFromTable(int row);
//    virtual QString orderByClause() const;
//    virtual QString selectStatement() const;

//    void setPrimaryKey(const QSqlIndex &key);
//    void setQuery(const QSqlQuery &query);
//    QModelIndex indexInQuery(const QModelIndex &item) const override;
//    QSqlRecord primaryValues(int row) const;
};

#endif // QSQLTABLEMODEL_H
