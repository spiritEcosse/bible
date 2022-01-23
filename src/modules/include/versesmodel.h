#ifndef VERSESMODEL_H
#define VERSESMODEL_H

#include <QSqlTableModel>
#include <QSqlRecord>

class VersesModel : public QSqlTableModel
{
    Q_OBJECT
public:
    VersesModel(QSqlDatabase db, QObject *parent = 0);
    VersesModel(QObject *parent = 0);
    ~VersesModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const;
    void getByBookAndChapter(const quint16 &book_number, const quint16 &chapter);
    void reset();
private:
    const static char* SQL_SELECT;
    void generateRoleNames();
    QHash<int, QByteArray> m_roleNames;
};

#endif // VERSESMODEL_H
