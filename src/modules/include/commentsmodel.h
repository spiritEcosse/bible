#ifndef COMMENTSMODEL_H
#define COMMENTSMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include "dbmanager.h"

class CommentsModel : public QSqlTableModel
{
    Q_OBJECT
public:
    enum CommentRoles {
        Text = Qt::UserRole + 1,
        Marker = Qt::UserRole + 2,
        VerseNumberFrom = Qt::UserRole + 3
    };
    CommentsModel(QSqlDatabase db, int currentBook, int currentChapter, QObject *parent = Q_NULLPTR);
    CommentsModel(QObject *parent = Q_NULLPTR);
    ~CommentsModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const;
    void updateObjects();
private:
    int m_currentBook;
    int m_currentChapter;
};

#endif // COMMENTSMODEL_H
