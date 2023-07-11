//
// Created by ihor on 17.12.2022.
//

#ifndef MODELCOMMENT_H
#define MODELCOMMENT_H

#include "modelupdate.h"
#include <QObject>

namespace modules {

    class ModelComment : public ListModel<Comment, db::TranslationCommentStorage> {
        Q_OBJECT
      public:
        enum VerseNamesRoles { Text = 0, Marker = 1, VerseNumberFrom = 2 };

        explicit ModelComment(QString &&fileName = "", QObject *parent = nullptr);
        explicit ModelComment(int bookNumber,
                              int chapterNumber,
                              int verseNumber,
                              QString &&fileName = "",
                              QObject *parent = nullptr);
        virtual QHash<int, QByteArray> roleNames() const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;

      private:
        int m_chapterNumber{};
        int m_bookNumber{};
        int m_verseNumber{};
        void updateObjects();
    };

}  // namespace modules

#endif  //MODELCOMMENT_H
