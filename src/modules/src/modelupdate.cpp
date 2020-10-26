#include "modelupdate.h"

namespace modules {

    template <class T>
    ModelUpdate<T>::ModelUpdate()
        : m_db { db::Db<T>::getInstance() }
    {
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
            emit error("An error occured.");
        }
    }

    template class ModelUpdate<GroupModules>;
    template class ModelUpdate<Registry>;
}
