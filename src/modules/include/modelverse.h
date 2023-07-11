//
// Created by ihor on 21.11.2022.
//

#ifndef MODELVERSE_H
#define MODELVERSE_H

#include "modelupdate.h"
#include <QObject>

namespace modules {

    class ModelVerse : public ListModel<Verse, db::TranslationStorage> {
        Q_OBJECT
      public:
        enum VerseNamesRoles { BookNumber = 0, Chapter = 1, NumberVerse = 2, Text = 3, Comments = 4 };

        explicit ModelVerse(QString &&fileName = "", QObject *parent = nullptr);
        explicit ModelVerse(int bookNumber, int chapterNumber, QString &&fileName = "", QObject *parent = nullptr);
        virtual QHash<int, QByteArray> roleNames() const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;

      private:
        int m_chapterNumber{};
        int m_bookNumber{};
        void updateObjects();
    };

}  // namespace modules

#endif  // MODELVERSE_H
