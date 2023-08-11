#ifndef MODELUPDATE_H
#define MODELUPDATE_H

#include "db.h"
#include <QAbstractListModel>
#include <QObject>

namespace modules {

    using namespace sqlite_orm;

    template<class T, typename S = db::Storage>
    class BaseModel {
    public:
        explicit BaseModel();
        explicit BaseModel(QString &&fileName);
        ~BaseModel() = default;
        std::unique_ptr<db::Db<T, S>> m_db;

        template<class ColumnAssign, class ValueAssign, class ColumnIn, class Array>
        inline void updateAllIn(const ColumnAssign &columnAssign,
                                const ValueAssign &valueAssign,
                                const ColumnIn &columnIn,
                                const Array &array) const {
            m_db->storage->update_all(set(assign(columnAssign, valueAssign)), where(in(columnIn, array)));
        }
    };

    class QJsonListModel : public QAbstractListModel {
        Q_OBJECT
    public:
        explicit QJsonListModel(QObject *parent = nullptr);
    protected slots:

        virtual inline void updateObjectsFromJson() {
            updateWrapper();
        }

    protected:
        virtual void updateWrapper(){};
    signals:
        void error(const QString &error);
        void updateDone();
        void transformSuccess();
    };

    template<class T, typename S = db::Storage>
    class ListModel : public QJsonListModel, public BaseModel<T, S> {
    public:
        explicit ListModel(QString &&fileName = "", QObject *parent = nullptr);
        std::vector<std::unique_ptr<T>> m_objects;
        virtual int rowCount(const QModelIndex &parent) const override;
        virtual int rowCount() const;
        int objectsCount = 0;

    protected:
        Q_INVOKABLE bool canFetchMore([[maybe_unused]] const QModelIndex &parent) const override;
        Q_INVOKABLE bool canFetchMore() const;
        Q_INVOKABLE void fetchMore([[maybe_unused]] const QModelIndex &parent) override;
    };

    template<class T, typename S = db::Storage>
    class ModelUpdate : public ListModel<T, S> {
    public:
        explicit ModelUpdate(QString &&fileName = "", QObject *parent = nullptr);
        virtual void update(const std::vector<T> &container);
        virtual void updateUnique(const std::vector<std::unique_ptr<T>> &container);
        virtual void transform(const QJsonDocument &document);

        using BaseModel<T, S>::m_db;
        std::vector<T> m_objectsFromJson;
        using QJsonListModel::error;
        using QJsonListModel::transformSuccess;
        using QJsonListModel::updateDone;

    protected:
        void updateWrapper() override;
        const QString nameJson;

        virtual QString getNameJson() {
            return {};
        };
    };
}  // namespace modules

#endif  // MODELUPDATE_H
