//
// Created by ihor on 21.11.2022.
//

#ifndef MODELBOOK_H
#define MODELBOOK_H

#include "modelupdate.h"
#include <QObject>
#include <memory>
#include <QTimer>

namespace modules {

    namespace tests {
        class tst_ModelBook;
    }

    class ModelBook : public ListModel<Book, db::TranslationStorage> {
        Q_OBJECT
      public:
        enum BookRoles {
            BookNumber = 0,
            ShortName = 1,
            LongName = 2,
            BookColor = 3,
            IsPresent = 4,
            Chapters = 5,
            NumberChapters = 6,
            FoundVerses = 7
        };

        explicit ModelBook(QString &&fileName = "", QObject *parent = nullptr);
        explicit ModelBook(bool search, QString &&fileName = "", QObject *parent = nullptr);
        virtual QHash<int, QByteArray> roleNames() const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;
        Q_INVOKABLE virtual void searchVersesByText(const QString &searchVerseText);
        std::unique_ptr<QTimer> m_queryTimer = nullptr;

      private:
        friend tests::tst_ModelBook;
        int m_waitingTimeBeforeHitDb = 500;

        std::shared_ptr<QString> m_searchQueryInVerseText = std::make_shared<QString>("");
        void updateObjects();
    private slots:
        virtual void doSearchVersesByText();
    };

}  // namespace modules
#endif  // MODELBOOK_H
