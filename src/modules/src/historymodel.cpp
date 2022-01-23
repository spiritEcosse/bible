#include "historymodel.h"
#include <QDebug>
#include <QDateTime>

static const char *historyTableName = "history";

static void createTable()
{
    if (QSqlDatabase::database().tables().contains(historyTableName)) {
        // The table already exists; we don't need to do anything.
        return;
    }

    QSqlQuery query;

    if (!query.exec(
                "CREATE TABLE IF NOT EXISTS 'history' ("
                "   'testament_index'   INTEGER NOT NULL, "
                "   'timestamp'         TEXT NOT NULL, "
                "   'book_short_name'   TEXT NOT NULL, "
                "   'book_index'        INTEGER NOT NULL, "
                "   'chapter_index'     INTEGER NOT NULL, "
                "   'verse_index'       INTEGER NOT NULL)")) {
        qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }
}

HistoryModel::HistoryModel(QObject *parent)
    : QSqlTableModel(parent)
{
    createTable();
    setTable(historyTableName);
    setSort(1, Qt::DescendingOrder);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();

    if (rowCount()) {
        QSqlRecord recordFirst = record(0);

        m_testamentIndex = recordFirst.value("testament_index").toInt();
        emit changeTestamentIndex();
        m_bookIndex = recordFirst.value("book_index").toInt();
        emit changeBookIndex();
        m_chapterIndex = recordFirst.value("chapter_index").toInt();
        emit changeChapterIndex();
        m_verseIndex = recordFirst.value("verse_index").toInt();
        emit changeVerseIndex();
    }
}

HistoryModel::~HistoryModel()
{
}

QHash<int, QByteArray> HistoryModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "testament_index";
    names[Qt::UserRole + 1] = "timestamp";
    names[Qt::UserRole + 2] = "book_short_name";
    names[Qt::UserRole + 3] = "book_index";
    names[Qt::UserRole + 4] = "chapter_index";
    names[Qt::UserRole + 5] = "verse_index";
    return names;
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

bool HistoryModel::newRow()
{
    const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);

    QSqlRecord newRecord = record();
    newRecord.setValue("testament_index", testamentIndex());
    newRecord.setValue("book_short_name", bookShortName());
    newRecord.setValue("book_index", bookIndex());
    newRecord.setValue("chapter_index", chapterIndex());
    newRecord.setValue("verse_index", verseIndex());
    newRecord.setValue("timestamp", timestamp);

    if (!insertRecord(0, newRecord)) {
        qWarning() << "Failed to add new row: " << lastError().text();
        return false;
    }
    return submitAll();
}

int HistoryModel::testamentIndex() const
{
    return m_testamentIndex;
}

int HistoryModel::bookIndex() const
{
    return m_bookIndex;
}

QString HistoryModel::bookShortName() const
{
    return m_bookShortName;
}

int HistoryModel::chapterIndex() const
{
    return m_chapterIndex;
}

int HistoryModel::verseIndex() const
{
    return m_verseIndex;
}

void HistoryModel::setTestamentIndex(const int &testamentIndex)
{
    if (testamentIndex == m_testamentIndex) {
        return;
    }

    qDebug() << testamentIndex;
    m_testamentIndex = testamentIndex;
    m_bookIndex = -1;
    m_bookShortName = "";
    m_chapterIndex = -1;
    m_verseIndex = -1;
    emit changeTestamentIndex();
    emit changeBookIndex();
    emit changeChapterIndex();
    emit changeVerseIndex();
}

void HistoryModel::setBookIndex(const int &bookIndex)
{
    if (bookIndex == m_bookIndex) {
        return;
    }

    m_bookIndex = bookIndex;
    m_chapterIndex = -1;
    m_verseIndex = -1;
    newRow();
    emit changeBookIndex();
    emit changeChapterIndex();
    emit changeVerseIndex();
}

void HistoryModel::setBookShortName(const QString &bookShortName)
{
    if (bookShortName == m_bookShortName) {
        return;
    }

    m_bookShortName = bookShortName;
    setData(firstQModelIndex(2), m_bookShortName);
    submitAll();
    emit changeBookShortName();
}

void HistoryModel::setChapterIndex(const int &chapterIndex)
{
    if (chapterIndex == m_chapterIndex)
    {
        return;
    }

    if (m_chapterIndex != -1) {
        m_chapterIndex = chapterIndex;
        m_verseIndex = 1;
        newRow();
    } else {
        m_chapterIndex = chapterIndex;
        setData(firstQModelIndex(4), m_chapterIndex);
        submitAll();
        setVerseIndex(1);
    }

    emit changeVerseIndex();
    emit changeChapterIndex();
}

void HistoryModel::setVerseIndex(const int &verseIndex)
{
    if (verseIndex == m_verseIndex)
    {
        return;
    }

    m_verseIndex = verseIndex;
    setData(firstQModelIndex(5), m_verseIndex);
    submitAll();
    emit changeVerseIndex();
}

QModelIndex HistoryModel::firstQModelIndex(const int column) const
{
    return index(0, column, QModelIndex());
}

void HistoryModel::copyObject(
        const int &testamentIndex,
        const int &bookIndex,
        const int &chapterIndex,
        const int &verseIndex,
        const QString& bookShortName)
{
    setTestamentIndex(testamentIndex);
    setBookIndex(bookIndex);
    setChapterIndex(chapterIndex);
    setVerseIndex(verseIndex);
    setBookShortName(bookShortName);
}

void HistoryModel::addFromMask(
        const int &bookNumber,
        const int &chapterNumber,
        const int &verseNumber)
{
//    BooksModel book;
//    int bookIndex = book.getBookIndex(bookNumber);
    int testamentIndex = 0;

//    if (bookIndex > BooksModel::COUNT_BOOKS_OLD_TESTAMENT) {
//        testamentIndex = 1;
//        bookIndex -= BooksModel::COUNT_BOOKS_OLD_TESTAMENT;
//    }

    setTestamentIndex(testamentIndex);
//    setBookIndex(bookIndex - 1);
    setChapterIndex(chapterNumber);
    setVerseIndex(verseNumber);
}
