#include "modelupdate.h"
#include <QtDebug>

namespace modules {

    template <class T>
    ModelUpdate<T>::ModelUpdate()
        : m_db { db::Db<T>::getInstance() }
    {
    }

    template <class T>
    int ModelUpdate<T>::rowCount(const QModelIndex& parent) const
    {
        Q_UNUSED(parent)
        return static_cast<int>(m_objects.size());
    }

    template <class T>
    void ModelUpdate<T>::update(const std::vector<T>& container)
    {
        try {
          auto guard = m_db->storage->transaction_guard();

          m_db->removeAll();
          m_db->save(container.begin(), container.end());
          guard.commit();
          emit beginResetModel();
          m_objects = std::move(container);
          emit endResetModel();
          emit updateDone();
        } catch(const std::system_error& e) {
            qInfo() << e.what();
            emit error("An error occured.");
        }
    }

    template class ModelUpdate<GroupModules>;
    template class ModelUpdate<Module>;
    template class ModelUpdate<Registry>;
}
