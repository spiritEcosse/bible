#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>
#include "db.h"
#include <QObject>

namespace modules {

    template <class T>
    class Model : public QAbstractListModel
    {
    public:
        Model();
        std::unique_ptr<db::Db<T>> m_db;
        std::vector<std::unique_ptr<T>> m_objects;
        int objectsCount = 0;
        Q_INVOKABLE virtual int rowCount(const QModelIndex& parent = {}) const override;
    protected:
        Q_INVOKABLE bool canFetchMore(const QModelIndex &parent) const override;
        Q_INVOKABLE void fetchMore(const QModelIndex &parent) override;
    };

}

#endif // MODEL_H
