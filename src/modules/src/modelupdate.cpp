#include "modelupdate.h"
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "invaliddata.h"

namespace modules {

    template <class T>
    BaseModel<T>::BaseModel()
        : m_db { new db::Db<T>() } {}

    template<class T>
    BaseModel<T>::~BaseModel() {}

    Base::Base(QObject *parent) : QAbstractListModel(parent) {}

    template <class T>
    ModelUpdate<T>::ModelUpdate(QObject *parent)
        : Base(parent), BaseModel<T>() {}

    template <class T>
    int ModelUpdate<T>::rowCount(const QModelIndex& /* parent */) const
    {
        return objectsCount;
    }

    template <class T>
    bool ModelUpdate<T>::canFetchMore(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return false;
        return (objectsCount < static_cast<int>(m_objects.size()));
    }

    template <class T>
    void ModelUpdate<T>::fetchMore(const QModelIndex &parent)
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

    template<class T>
    void ModelUpdate<T>::updateWrapper()
    {
        update(m_objectsFromJson);
    }

    template <class T>
    void ModelUpdate<T>::update(const std::vector<T>& container)
    {
        try {
          auto guard = m_db->storage->transaction_guard();

          m_db->removeAll();
          int chunkSize = 1000;

          auto start = container.begin();
          auto end = container.end();

          while (start != end) {
              auto next = std::distance(start, end) >= chunkSize
                          ? start + chunkSize
                          : end;

              m_db->save(start, next);
              start = next;
          }
          guard.commit();
          emit updateDone();
        } catch(const std::system_error& e) {
            qInfo() << e.what();
            emit error("An error occured.");
        }
    }

    template <class T>
    void ModelUpdate<T>::updateUnique(const std::vector<std::unique_ptr<T>>& container)
    {
        try {
          auto guard = m_db->storage->transaction_guard();

          m_db->removeAll();
          int chunkSize = 1000;

          auto start = container.begin();
          auto end = container.end();

          while (start != end) {
              auto next = std::distance(start, end) >= chunkSize
                          ? start + chunkSize
                          : end;

              m_db->save(start, next);
              start = next;
          }
          guard.commit();
          emit updateDone();
        } catch(const std::system_error& e) {
            qInfo() << e.what();
            emit error("An error occured.");
        }
    }

    template <class T>
    void ModelUpdate<T>::transform(const QJsonDocument &document)
    {
        m_objectsFromJson.clear();
        try {
            const QJsonArray& source = document.object().value(getNameJson()).toArray();

            std::transform(source.begin(), source.end(), std::back_inserter(m_objectsFromJson),
                           [](const QJsonValue& entry)
            {
                return std::move(T(entry.toObject()));
            });
            emit transformSuccess();
        } catch(const core::InvalidData& e) {}
    }

    template class ModelUpdate<GroupModules>;
    template class ModelUpdate<Module>;
    template class ModelUpdate<Registry>;
    template class ModelUpdate<Host>;
    template class BaseModel<Module>;
    template class BaseModel<GroupModules>;
    template class BaseModel<Registry>;
    template class BaseModel<Host>;
}
