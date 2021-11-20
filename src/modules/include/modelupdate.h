#ifndef MODELUPDATE_H
#define MODELUPDATE_H

#include <QAbstractListModel>
#include "db.h"
#include <QObject>

namespace modules {

    using namespace sqlite_orm;

    template <class T>
    class BaseModel
    {
    public:
        BaseModel();
        ~BaseModel();
        std::unique_ptr<db::Db<T>> m_db;

        template<class ColumnAssign, class ValueAssign, class ColumnIn, class Array>
        inline void updateAllIn(const ColumnAssign& columnAssign, const ValueAssign &valueAssign,
                              const ColumnIn& columnIn, const Array& array) const
        {
            m_db->storage->update_all(
                        set(assign(columnAssign, valueAssign)),
                        where(in(columnIn, array)));
        }
    };

    class Base : public QAbstractListModel
    {
        Q_OBJECT
    public:
        Base(QObject *parent = nullptr);
    private:
        virtual void updateWrapper() = 0;
    signals:
        void error(const QString& error);
        void updateDone();
        void transformSuccess();
    protected slots:
        virtual inline void updateObjectsFromJson() { updateWrapper(); }
    };

    template <class T>
    class ModelUpdate: public Base, public BaseModel<T>
    {
    public:
        using BaseModel<T>::m_db;

        ModelUpdate(QObject *parent = nullptr);
        virtual void update(const std::vector<T>& container);
        virtual void updateUnique(const std::vector<std::unique_ptr<T>>& container);
        virtual int rowCount(const QModelIndex& parent = {}) const override;
        virtual void transform(const QJsonDocument& document);

        std::vector<std::unique_ptr<T>> m_objects;
        std::vector<T> m_objectsFromJson;
        int objectsCount = 0;
    protected:
        Q_INVOKABLE bool canFetchMore(const QModelIndex &parent) const override;
        Q_INVOKABLE void fetchMore(const QModelIndex &parent) override;
        const QString nameJson;
        virtual const QString getNameJson() { return QString(); };
    private:
        virtual void updateWrapper() override;
    };
}

#endif // MODELUPDATE_H
