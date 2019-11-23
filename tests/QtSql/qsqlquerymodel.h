#ifndef QSQLQUERYMODEL_H
#define QSQLQUERYMODEL_H

//#include <QtSql/qtsqlglobal.h>
//#include <QAbstractItemModel>
#include <QObject>
#include <QSqlError>
//#include <QModelIndex>
//#include <QSqlDatabase>

//QT_REQUIRE_CONFIG(sqlmodel);

class QSqlQueryModel : public QObject
{
    Q_OBJECT
public:
    explicit QSqlQueryModel(QObject *parent = nullptr) {}
    virtual ~QSqlQueryModel() {}

//    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
//    int columnCount(const QModelIndex &parent = QModelIndex()) const ;
//    QSqlRecord record(int row) const;
//    QSqlRecord record() const;

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const ;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const ;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                       int role = Qt::EditRole) ;

//    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) ;
//    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) ;

//    void setQuery(const QSqlQuery &query);
//    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
//    QSqlQuery query() const;

    virtual void clear() {}

    virtual QSqlError lastError() const {}

//    void fetchMore(const QModelIndex &parent = QModelIndex()) ;
//    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const ;

    QHash<int, QByteArray> roleNames() const ;

protected:
    void beginInsertRows(const QModelIndex &parent, int first, int last);
    void endInsertRows();

    void beginRemoveRows(const QModelIndex &parent, int first, int last);
    void endRemoveRows();

    void beginInsertColumns(const QModelIndex &parent, int first, int last);
    void endInsertColumns();

    void beginRemoveColumns(const QModelIndex &parent, int first, int last);
    void endRemoveColumns();

    void beginResetModel();
    void endResetModel();
    virtual void queryChange() {}

//    virtual QModelIndex indexInQuery(const QModelIndex &item) const {}
//    void setLastError(const QSqlError &error);
//    QSqlQueryModel(QSqlQueryModelPrivate &dd, QObject *parent = nullptr);
};

#endif // QSQLQUERYMODEL_H
