#include "modelupdate.h"
#include "invaliddata.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace modules {

    QJsonListModel::QJsonListModel(QObject *parent) : QAbstractListModel(parent) {}

    template<class T, typename S>
    BaseModel<T, S>::BaseModel() : m_db{new db::Db<T, S>()} {}

    template<class T, typename S>
    BaseModel<T, S>::BaseModel(QString &&fileName) : m_db{new db::Db<T, S>(std::move(fileName))} {}

    template<class T, typename S>
    ListModel<T, S>::ListModel(QString &&fileName, QObject *parent) :
        QJsonListModel(parent), BaseModel<T, S>(std::move(fileName)) {}

    template<class T, typename S>
    ModelUpdate<T, S>::ModelUpdate(QString &&fileName, QObject *parent) :
        ListModel<T, S>(std::move(fileName), parent) {}

    template<class T, typename S>
    int ListModel<T, S>::rowCount(const QModelIndex & /* parent */) const {
        return objectsCount;
    }

    template<class T, typename S>
    int ListModel<T, S>::rowCount() const {
        return objectsCount;
    }

    template<class T, typename S>
    bool ListModel<T, S>::canFetchMore([[maybe_unused]] const QModelIndex &parent) const {
        return (objectsCount < static_cast<int>(m_objects.size()));
    }

    template<class T, typename S>
    bool ListModel<T, S>::canFetchMore() const {
        return (objectsCount < static_cast<int>(m_objects.size()));
    }

    template<class T, typename S>
    void ListModel<T, S>::fetchMore([[maybe_unused]] const QModelIndex &parent) {
        int remainder = static_cast<int>(m_objects.size()) - objectsCount;
        int itemsToFetch = qMin(40, remainder);

        if(itemsToFetch <= 0)
            return;

        beginInsertRows(QModelIndex(), objectsCount, objectsCount + itemsToFetch - 1);
        objectsCount += itemsToFetch;
        endInsertRows();
    }

    template<class T, typename S>
    void ModelUpdate<T, S>::updateWrapper() {
        update(m_objectsFromJson);
    }

    template<class T, typename S>
    void ModelUpdate<T, S>::update(const std::vector<T> &container) {
        try {
            auto guard = m_db->storage->transaction_guard();

            m_db->removeAll();
            int chunkSize = 1000;

            auto start = container.begin();
            auto end = container.end();

            while(start != end) {
                auto next = std::distance(start, end) >= chunkSize ? start + chunkSize : end;

                m_db->save(start, next);
                start = next;
            }
            guard.commit();
            emit updateDone();
        } catch(const std::system_error &e) {
            qInfo() << e.what();
            emit error("An error occured.");
        }
    }

    template<class T, typename S>
    void ModelUpdate<T, S>::updateUnique(const std::vector<std::unique_ptr<T>> &container) {
        try {
            auto guard = m_db->storage->transaction_guard();

            m_db->removeAll();
            int chunkSize = 1000;

            auto start = container.begin();
            auto end = container.end();

            while(start != end) {
                auto next = std::distance(start, end) >= chunkSize ? start + chunkSize : end;

                m_db->save(start, next);
                start = next;
            }
            guard.commit();
            emit updateDone();
        } catch(const std::system_error &e) {
            qInfo() << e.what();
            emit error("An error occured.");
        }
    }

    template<class T, typename S>
    void ModelUpdate<T, S>::transform(const QJsonDocument &document) {
        m_objectsFromJson.clear();
        try {
            const QJsonArray &source = document.object().value(getNameJson()).toArray();

            std::transform(source.begin(),
                           source.end(),
                           std::back_inserter(m_objectsFromJson),
                           [](const QJsonValue &entry) {
                               return std::move(T(entry.toObject()));
                           });
            emit transformSuccess();
        } catch(const core::InvalidData &e) {
        }
    }

    template class ModelUpdate<GroupModules>;
    template class ModelUpdate<Module>;
    template class ModelUpdate<Registry>;
    template class ModelUpdate<Host>;
    template class BaseModel<Module>;
    template class BaseModel<GroupModules>;
    template class BaseModel<Registry>;
    template class BaseModel<Host>;
    template class BaseModel<Book, db::TranslationStorage>;
    template class ListModel<Book, db::TranslationStorage>;
    template class BaseModel<Verse, db::TranslationStorage>;
    template class ListModel<Verse, db::TranslationStorage>;
    template class BaseModel<Comment, db::TranslationCommentStorage>;
    template class ListModel<Comment, db::TranslationCommentStorage>;
    template class ListModel<Record>;
    template class BaseModel<Record>;
}  // namespace modules
