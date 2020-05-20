#ifndef QSQLRECORD_H
#define QSQLRECORD_H

#include <QVariant>

class QSqlRecord
{
public:
    QSqlRecord() {}
    QSqlRecord(const QSqlRecord&) {}
    virtual ~QSqlRecord() {}

//    bool operator==(const QSqlRecord &other) const;
//    inline bool operator!=(const QSqlRecord &other) const { return !operator==(other); }

    virtual QVariant value(int) const { return QVariant(); }
//    QVariant value(const QString& name) const;
//    void setValue(int i, const QVariant& val);
    virtual void setValue(const QString&, const QVariant&) {}

//    void setNull(int i);
//    void setNull(const QString& name);
//    bool isNull(int i) const;
//    bool isNull(const QString& name) const;

//    int indexOf(const QString &name) const;
//    QString fieldName(int i) const;

//    QSqlField field(int i) const;
//    QSqlField field(const QString &name) const;

//    bool isGenerated(int i) const;
//    bool isGenerated(const QString& name) const;
//    void setGenerated(const QString& name, bool generated);
//    void setGenerated(int i, bool generated);

//    void append(const QSqlField& field);
//    void replace(int pos, const QSqlField& field);
//    void insert(int pos, const QSqlField& field);
//    void remove(int pos);

//    bool isEmpty() const;
//    bool contains(const QString& name) const;
//    void clear();
//    void clearValues();
//    int count() const;
//    QSqlRecord keyValues(const QSqlRecord &keyFields) const;

//private:
//    void detach();
//    QSqlRecordPrivate* d;
};

#endif // QSQLRECORD_H