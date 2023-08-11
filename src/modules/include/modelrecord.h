//
// Created by ihor on 20.12.2022.
//

#ifndef MODELRECORD_H
#define MODELRECORD_H

#include "modelupdate.h"
#include <QObject>

namespace modules {
    class ModelRecord : public ListModel<Record> {
        Q_OBJECT
        Q_PROPERTY(int bookIndex READ bookIndex WRITE setBookIndex NOTIFY changeBookIndex)
        Q_PROPERTY(int chapterIndex READ chapterIndex WRITE setChapterIndex NOTIFY changeChapterIndex)
        Q_PROPERTY(int verseIndex READ verseIndex WRITE setVerseIndex NOTIFY changeVerseIndex)
    public:
        enum RecordRoles { Timestamp = 0, BookShortName = 1, BookIndex = 2, ChapterIndex = 3, VerseIndex = 4 };

        explicit ModelRecord();
        [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
        Q_INVOKABLE void createRecord(QString bookShortName,
                                      int bookIndex,
                                      int chapterIndex,
                                      int verseIndex,
                                      QDateTime timestamp = QDateTime::currentDateTime());
        static void registerMe();
        int m_bookIndex = -1;
        int m_chapterIndex = -1;
        int m_verseIndex = -1;

        int bookIndex() const;
        int chapterIndex() const;
        int verseIndex() const;
        void setBookIndex(int bookIndex);
        void setChapterIndex(int chapterIndex);
        void setVerseIndex(int verseIndex);
    signals:
        void changeBookIndex();
        void changeChapterIndex();
        void changeVerseIndex();

    private:
        void updateObjects();
    };
}  // namespace modules
#endif  //MODELRECORD_H
