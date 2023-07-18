//
// Created by ihor on 06.12.2022.
//

#ifndef MODELCHAPTER_H
#define MODELCHAPTER_H

#include "modelupdate.h"
#include <QObject>
#include <memory>

namespace modules {

    namespace tests {
        class tst_ModelChapter;
    }

    class ModelChapter : public ListModel<Verse, db::TranslationStorage> {
        Q_OBJECT
      public:
        enum ChapterRoles { BookNumber = 0, Chapter = 1, NumberVerses = 2, Verses = 3 };

        explicit ModelChapter(QString &&fileName = "", QObject *parent = nullptr);
        explicit ModelChapter(int bookNumber, QString &&fileName = "", QObject *parent = nullptr);
        virtual QHash<int, QByteArray> roleNames() const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;

      private:
        friend tests::tst_ModelChapter;
        void updateObjects();
        int m_bookNumber{};
    };
}  // namespace modules

#endif  // MODELCHAPTER_H
