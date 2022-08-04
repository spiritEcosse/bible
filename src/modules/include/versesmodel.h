#ifndef VERSESMODEL_H
#define VERSESMODEL_H

#include <QSqlTableModel>
#include <QSqlRecord>
#include <memory>
#include "commentsmodel.h"

class VersesModel : public QSqlTableModel
{
    Q_OBJECT
public:
    enum VerseRoles {
        Text = 0,
        Verse = 1,
        Comments = 2
    };
    VersesModel(QSqlDatabase db, QObject *parent = 0);
    VersesModel(QObject *parent = 0);
    ~VersesModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const;
    void getByBookAndChapter(const quint16 &book_number, const quint16 &chapter);
    void reset();
private:
    const static char* SQL_SELECT;
    QHash<int, QByteArray> m_roleNames;
    std::shared_ptr<CommentsModel> m_comments;
};

#endif // VERSESMODEL_H
