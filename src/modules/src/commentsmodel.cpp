#include "commentsmodel.h"
#include <QDebug>

static const char *commentariesTableName = "commentaries";

CommentsModel::CommentsModel(QSqlDatabase db, int currentBook, int currentChapter, QObject *parent)
    : QSqlTableModel(parent, db),
      m_currentBook (currentBook),
      m_currentChapter (currentChapter)
{
    updateObjects();
}

CommentsModel::CommentsModel(QObject *parent)
    : QSqlTableModel(parent)
{
}

CommentsModel::~CommentsModel()
{
}

QHash<int, QByteArray> CommentsModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Text] = "text";
    names[Marker] = "marker";
    names[VerseNumberFrom] = "verse_number_from";
    return names;
}

QVariant CommentsModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if(role < Qt::UserRole) {
        value = QSqlQueryModel::data(index, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}

void CommentsModel::updateObjects()
{
    setTable(commentariesTableName);

    setFilter(QString("book_number='%1' AND chapter_number_from='%2'").arg(QString::number(m_currentBook),QString::number(m_currentChapter)));

    if (! select()) {
        qWarning() << lastError().text();
    }
}
