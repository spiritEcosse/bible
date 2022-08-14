#include "versesmodel.h"
#include "dbmanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QtQuick>

VersesModel::VersesModel(QSqlDatabase db, QObject *parent)
    : QSqlTableModel(parent, db)
{
}

VersesModel::VersesModel(QObject *parent)
{

}

VersesModel::~VersesModel()
{
}

QHash<int, QByteArray> VersesModel::roleNames() const
{
    return {
        { Text, "verse_text" },
        { Verse, "verse_number" },
        { Comments, "comments" },
    };
}

const char* VersesModel::SQL_SELECT =
        "SELECT verses.text as verse_text, verses.verse as verse_number FROM verses WHERE "
        "verses.book_number = %1 AND verses.chapter = %2";

void VersesModel::getByBookAndChapter(const quint16 &book_number, const quint16 &chapter, const QString& abbrModule)
{
    QString sql;
    sql = QString(SQL_SELECT).arg(QString::number(book_number), QString::number(chapter));
    this->setQuery(QSqlQuery(sql, database()));

    DbManagerComments db_comments (abbrModule);
    m_comments.reset(new CommentsModel(db_comments.db, book_number, chapter));
}

QVariant VersesModel
::data(const QModelIndex &index, int role) const
{
    QVariant value;

    switch(role) {
        case Comments:         
#ifdef Qt6_FOUND
            value = QVariant::fromValue(m_comments.get());
#else
            value = qVariantFromValue(m_comments.get());
#endif
        break;
    default:
        if(role < Qt::UserRole) {
            value = QSqlQueryModel::data(index, role);
        } else {
            int columnIdx = role - Qt::UserRole - 1;
            QModelIndex modelIndex = this->index(index.row(), columnIdx);
            value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
        }
    }
    return value;
}

void VersesModel::reset(const QString& abbrModule)
{
    beginResetModel();
    getByBookAndChapter(0, 0, abbrModule);
    endResetModel();
}
