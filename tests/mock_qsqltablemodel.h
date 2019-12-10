#ifndef MOCK_QSQLTABLEMODEL_H
#define MOCK_QSQLTABLEMODEL_H

#include <gmock/gmock.h>

#include <QSqlTableModel>


class MockQSqlTableModel : public QSqlTableModel
{
public:
  MOCK_METHOD1(setTable, void(const QString &));
  MOCK_CONST_METHOD1(record, QSqlRecord(int));
  MOCK_CONST_METHOD0(record, QSqlRecord());
  MOCK_CONST_METHOD2(data, QVariant(const QModelIndex &, int role ));
  MOCK_METHOD2(insertRecord, bool(int row, const QSqlRecord &record));
  MOCK_METHOD0(select, bool());
  MOCK_METHOD0(submitAll, bool());
  MOCK_METHOD0(database, QSqlDatabase&());
  MOCK_METHOD3(removeRows, bool(int row, int count, const QModelIndex &parent ));
};

#endif // MOCK_QSQLTABLEMODEL_H
