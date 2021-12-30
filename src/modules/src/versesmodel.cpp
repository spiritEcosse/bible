#include "versesmodel.h"
#include "dbmanager.h"
#include <QDebug>
#include <QSqlQuery>

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
    return m_roleNames;
}

const char* VersesModel::SQL_SELECT =
        "SELECT verses.text as verse_text, verses.verse as verse_number FROM verses WHERE "
        "verses.book_number = %1 AND verses.chapter = %2";

void VersesModel::getByBookAndChapter(const quint16 &book_number, const quint16 &chapter)
{
    QString sql;
    sql = QString(SQL_SELECT).arg(QString::number(book_number), QString::number(chapter));
    qDebug() << "Sdsd";
    this->setQuery(QSqlQuery(sql, database()));
    qDebug() << "Sdsd";
    generateRoleNames();
}

QVariant VersesModel::data(const QModelIndex &index, int role) const
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

void VersesModel::generateRoleNames()
{
    m_roleNames.clear();

    for( int i = 0; i < record().count(); i ++) {
        m_roleNames.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8());
    }
}

void VersesModel::reset()
{
    beginResetModel();
    getByBookAndChapter(0, 0);
    endResetModel();
}
