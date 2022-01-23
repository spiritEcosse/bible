#ifndef HISTORY_H
#define HISTORY_H

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

#include "booksmodel.h"

class HistoryModel : public QSqlTableModel
{
    Q_OBJECT
    Q_PROPERTY(int testamentIndex READ testamentIndex WRITE setTestamentIndex NOTIFY changeTestamentIndex)
    Q_PROPERTY(int bookIndex READ bookIndex WRITE setBookIndex NOTIFY changeBookIndex)
    Q_PROPERTY(QString bookShortName READ bookShortName WRITE setBookShortName NOTIFY changeBookShortName)
    Q_PROPERTY(int chapterIndex READ chapterIndex WRITE setChapterIndex NOTIFY changeChapterIndex)
    Q_PROPERTY(int verseIndex READ verseIndex WRITE setVerseIndex NOTIFY changeVerseIndex)
public:
    HistoryModel(QObject *parent = 0);
    ~HistoryModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const;
    bool newRow();
    int testamentIndex() const;
    int bookIndex() const;
    QString bookShortName() const;
    int chapterIndex() const;
    int verseIndex() const;
    void setTestamentIndex(const int &testamentIndex);
    void setBookIndex(const int &bookIndex);
    void setBookShortName(const QString &bookShortName);
    void setChapterIndex(const int &chapterIndex);
    void setVerseIndex(const int &verseIndex);
    Q_INVOKABLE void copyObject(const int &testamentIndex, const int &bookIndex, const int &chapterIndex, const int &verseIndex, const QString& bookShortName);
    Q_INVOKABLE void addFromMask(const int &bookNumber, const int &chapterNumber, const int &verseNumber);
signals:
    void changeTestamentIndex();
    void changeBookIndex();
    void changeBookShortName();
    void changeChapterIndex();
    void changeVerseIndex();
private:
    int m_testamentIndex = -1;
    int m_bookIndex = -1;
    QString m_bookShortName;
    int m_chapterIndex = -1;
    int m_verseIndex = -1;
    QModelIndex firstQModelIndex(const int column) const;
};

#endif // HISTORY_H
