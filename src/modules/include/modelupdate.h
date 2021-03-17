#ifndef MODELUPDATE_H
#define MODELUPDATE_H

#include <QAbstractListModel>
#include "db.h"

namespace modules {

    class Base : public QAbstractListModel
    {
        Q_OBJECT
    signals:
        void error(const QString& error);
        void updateDone();
    };

    template <class T>
    class ModelUpdate: public Base
    {
    public:
        ModelUpdate();
        void update(const std::vector<T>& container);
        virtual int rowCount(const QModelIndex& parent = {}) const override;

        std::vector<T> m_objects = {};
        std::shared_ptr<db::Db<T>> m_db = db::Db<T>::getInstance();
    };

}

#endif // MODELUPDATE_H
