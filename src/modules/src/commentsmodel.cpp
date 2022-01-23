#include "commentsmodel.h"
#include <QDebug>

static const char *commentariesTableName = "commentaries";

CommentsModel::CommentsModel(QObject *parent)
    : QSqlTableModel(parent)
{
    setTable(commentariesTableName);
}

CommentsModel::~CommentsModel()
{
}

QHash<int, QByteArray> CommentsModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "text";
    return names;
}

QVariant CommentsModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

int CommentsModel::currentBook() const
{
    return m_currentBook;
}

int CommentsModel::currentChapter() const
{
    return m_currentChapter;
}

int CommentsModel::currentVerse() const
{
    return m_currentVerse;
}

QString CommentsModel::currentMarker() const
{
    return m_currentMarker;
}

QString CommentsModel::currentText() const
{
    return m_currentText;
}

void CommentsModel::setCurrentBook(const int &currentBook)
{
    if (m_currentBook == currentBook) {
        return;
    }

    m_currentBook = currentBook;
}

void CommentsModel::setCurrentChapter(const int &currentChapter)
{
    if (m_currentChapter == currentChapter) {
        return;
    }

    m_currentChapter = currentChapter;
}

void CommentsModel::setCurrentVerse(const int &currentVerse)
{
    if (m_currentVerse == currentVerse) {
        return;
    }

    m_currentVerse = currentVerse;
}

void CommentsModel::setCurrentMarker(const QString &currentMarker)
{
    if (m_currentMarker == currentMarker) {
        return;
    }

    m_currentMarker = currentMarker;

    setFilter(QString("book_number='%1' AND chapter_number_from='%2' AND marker='%3'").arg(QString::number(currentBook()), QString::number(currentChapter()), m_currentMarker));

    if (select()) {
        QSqlRecord recordFilter = record(0);
        m_currentText = recordFilter.value("text").toString();
        emit changeCurrentText();
    } else {
        qWarning() << lastError().text();
    }
}
