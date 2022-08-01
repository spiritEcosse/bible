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
    Q_PROPERTY(int currentBook READ currentBook WRITE setCurrentBook)
    Q_PROPERTY(int currentChapter READ currentChapter WRITE setCurrentChapter)
    Q_PROPERTY(int currentVerse READ currentVerse WRITE setCurrentVerse)
    Q_PROPERTY(QString currentText READ currentText NOTIFY changeCurrentText)
public:
    CommentsModel(QSqlDatabase db, QObject *parent = Q_NULLPTR);
    CommentsModel(QObject *parent = Q_NULLPTR);
    ~CommentsModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const;
    int currentBook() const;
    int currentChapter() const;
    int currentVerse() const;
    QString currentMarker() const;
    QString currentText() const;
    void setCurrentBook(const int &currentBook);
    void setCurrentChapter(const int &currentChapter);
    void setCurrentVerse(const int &currentVerse);
    void updateObjects();
signals:
    void changeCurrentText();
private:
    int m_currentBook;
    int m_currentChapter;
    int m_currentVerse;
    QString m_currentMarker;
    QString m_currentText;
};

#endif // COMMENTSMODEL_H
