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
        void updateUnique(const std::vector<std::unique_ptr<T>>& container);
        virtual int rowCount(const QModelIndex& parent = {}) const override;

        std::vector<std::unique_ptr<T>> m_objects;
        int objectsCount = 0;
        std::shared_ptr<db::Db<T>> m_db = db::Db<T>::getInstance();
    protected:
        Q_INVOKABLE bool canFetchMore(const QModelIndex &parent) const override;
        Q_INVOKABLE void fetchMore(const QModelIndex &parent) override;
    };

}

#endif // MODELUPDATE_H
