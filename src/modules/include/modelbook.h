//
// Created by ihor on 21.11.2022.
//

#ifndef MODELBOOK_H
#define MODELBOOK_H

#include "modelupdate.h"
#include <QObject>
#include <memory>

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
            NumberChapters = 6
        };

        explicit ModelBook(QString &&fileName = "", QObject *parent = nullptr);
        virtual QHash<int, QByteArray> roleNames() const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;

      private:
        friend tests::tst_ModelBook;

        void updateObjects();
    };

}  // namespace modules
#endif  // MODELBOOK_H
