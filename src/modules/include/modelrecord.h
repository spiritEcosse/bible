//
// Created by ihor on 20.12.2022.
//

#ifndef MODELRECORD_H
#define MODELRECORD_H

#include "modelupdate.h"
#include <QObject>
#include "modelmodule.h"

namespace modules {

    namespace tests {
        class tst_ModelRecord;
    }  // namespace tests

    class ModelRecord : public ListModel<Record> {
        Q_OBJECT
        Q_PROPERTY(int bookIndex READ bookIndex WRITE setBookIndex NOTIFY changeBookIndex)
        Q_PROPERTY(int chapterIndex READ chapterIndex WRITE setChapterIndex NOTIFY changeChapterIndex)
        Q_PROPERTY(int verseIndex READ verseIndex WRITE setVerseIndex NOTIFY changeVerseIndex)
    public:
        enum RecordRoles {
            Timestamp = 0,
            BookNumber = 1,
            BookIndex = 2,
            ChapterIndex = 3,
            VerseIndex = 4,
            BookShortName = 5
        };

        explicit ModelRecord();
        ~ModelRecord() noexcept;
        [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
        Q_INVOKABLE void createRecord(int bookNumber,
                                      int bookIndex,
                                      int chapterIndex,
                                      int verseIndex,
                                      QDateTime timestamp = QDateTime::currentDateTime());
        Q_INVOKABLE void updateObjects();
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
        friend class tests::tst_ModelRecord;

        sqlite3 *db;
        std::unique_ptr<ModelModule> m_modelModule = nullptr;
    };
}  // namespace modules
#endif  //MODELRECORD_H
