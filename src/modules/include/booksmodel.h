#ifndef BOOKSMODEL_H
#define BOOKSMODEL_H

#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include "versesmodel.h"
#include "historymodel.h"
#include "dbmanager.h"

class BooksModel : public QSqlTableModel
{
    Q_OBJECT
    Q_PROPERTY(int currentBook READ currentBook WRITE setCurrentBook NOTIFY changeCurrentBook)
    Q_PROPERTY(int currentChapter READ currentChapter WRITE setCurrentChapter NOTIFY changeCurrentChapter)
    Q_PROPERTY(VersesModel* currentVerses READ currentVerses NOTIFY changeCurrentVerses)
public:
    BooksModel(QSqlDatabase db, QObject *parent = Q_NULLPTR);
    BooksModel(QObject *parent = Q_NULLPTR);
    static const int COUNT_BOOKS_OLD_TESTAMENT = 39;

    Q_INVOKABLE void oldTestament();
    Q_INVOKABLE void newTestament();
    int getBookIndex(const int &bookNumber);
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const;

    int currentBook() const;
    int currentChapter() const;
    VersesModel* currentVerses();
    void setCurrentBook(const int &currentBook);
    void setCurrentChapter(const int &currentChapter);
    void setCurrentVerses();
signals:
    void changeCurrentBook();
    void changeCurrentChapter();
    void changeCurrentVerses();
private:
    int m_currentBook = 0;
    int m_currentChapter = 0;
    VersesModel m_currentVerses;
    const static char* SQL_SELECT;
    void generateRoleNames();
    QHash<int, QByteArray> m_roleNames;
};

#endif // BOOKSMODEL_H
