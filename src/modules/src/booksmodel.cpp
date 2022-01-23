#include "booksmodel.h"
#include <QDebug>

BooksModel::BooksModel(QSqlDatabase db, QObject *parent)
    : QSqlTableModel(parent, db),
      m_currentVerses (db)
{
    m_currentVerses.reset();
    setQuery(QSqlQuery(SQL_SELECT, database()));
    generateRoleNames();
}

BooksModel::BooksModel(QObject *parent)
    : QSqlTableModel(parent)
{

}

const char* BooksModel::SQL_SELECT =
        "SELECT books.short_name, books.long_name, books.book_number, "
        "(SELECT MAX(verses.chapter) "
        "   FROM verses WHERE verses.book_number == books.book_number) AS chapters "
        "FROM books ORDER BY books.book_number ASC";

QHash<int, QByteArray> BooksModel::roleNames() const
{
  return m_roleNames;
}

QVariant BooksModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if(role < Qt::UserRole) {
        value = QSqlTableModel::data(index, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    }

    return value;
}

void BooksModel::oldTestament()
{
    QString sql = QString(SQL_SELECT).arg(
                QString::number(0), QString::number(COUNT_BOOKS_OLD_TESTAMENT));
    setQuery(QSqlQuery(sql, database()));
    generateRoleNames();
}

void BooksModel::newTestament()
{
    QString sql = QString(SQL_SELECT).arg(
                QString::number(COUNT_BOOKS_OLD_TESTAMENT), QString::number(-1));
    setQuery(QSqlQuery(sql, database()));
    generateRoleNames();
}

void BooksModel::generateRoleNames()
{
    m_roleNames.clear();

    for( int i = 0; i < record().count(); i ++) {
        m_roleNames.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8());
    }
}

int BooksModel::currentBook() const {
    return m_currentBook;
}

int BooksModel::currentChapter() const {
    return m_currentChapter;
}

VersesModel* BooksModel::currentVerses()
{
    return &m_currentVerses;
}

void BooksModel::setCurrentBook(const int &currentBook)
{
    if (currentBook == m_currentBook) {
        return;
    }

    m_currentBook = currentBook;
    emit changeCurrentBook();
    m_currentChapter = -1;
    emit changeCurrentChapter();
    m_currentVerses.clear();
    emit changeCurrentVerses();
}

void BooksModel::setCurrentChapter(const int &currentChapter)
{
    if (currentChapter == m_currentChapter) {
        return;
    }

    m_currentChapter = currentChapter;
    emit changeCurrentChapter();
    setCurrentVerses();
}

void BooksModel::setCurrentVerses()
{
    m_currentVerses.clear();
    m_currentVerses.getByBookAndChapter(currentBook(), currentChapter());
    emit changeCurrentVerses();
}

int BooksModel::getBookIndex(const int &bookNumber)
{
    QString sql = QString("SELECT rowid FROM books WHERE book_number='%1' ").arg(bookNumber);
    setQuery(QSqlQuery(sql, database()));
    QSqlQuery queryExec = query();

    if (queryExec.exec())
        return queryExec.value(0).toInt();

    qFatal("Cannot read from books: %s", qPrintable(queryExec.lastError().text()));
}
