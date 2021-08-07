#include "model.h"

namespace modules {

    template <class T>
    Model<T>::Model()
        : m_db { new db::Db<T>() } {}

    template <class T>
    int Model<T>::rowCount(const QModelIndex& /* parent */) const
    {
        return objectsCount;
    }

    template <class T>
    bool Model<T>::canFetchMore(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return false;
        return (objectsCount < static_cast<int>(m_objects.size()));
    }

    template <class T>
    void Model<T>::fetchMore(const QModelIndex &parent)
    {
        if (parent.isValid())
            return;
        int remainder = static_cast<int>(m_objects.size()) - objectsCount;
        int itemsToFetch = qMin(40, remainder);

        if (itemsToFetch <= 0)
            return;

        beginInsertRows(QModelIndex(), objectsCount, objectsCount + itemsToFetch - 1);
        objectsCount += itemsToFetch;
        endInsertRows();
    }

}
