#ifndef MODELUPDATE_H
#define MODELUPDATE_H

#include <QAbstractListModel>
#include "db.h"
#include <QObject>

namespace modules {

    class Base : public QAbstractListModel
    {
        Q_OBJECT
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
    class ModelUpdate: public Base
    {
    public:
        ModelUpdate();
        virtual void update(const std::vector<T>& container);
        virtual void updateUnique(const std::vector<std::unique_ptr<T>>& container);
        virtual int rowCount(const QModelIndex& parent = {}) const override;
        virtual void transform(const QJsonDocument& document);

        std::vector<std::unique_ptr<T>> m_objects;
        std::vector<T> m_objectsFromJson;
        int objectsCount = 0;
        std::unique_ptr<db::Db<T>> m_db;
    protected:
        Q_INVOKABLE bool canFetchMore(const QModelIndex &parent) const override;
        Q_INVOKABLE void fetchMore(const QModelIndex &parent) override;
        const QString nameJson;
        virtual const QString getNameJson() = 0;
    private:
        virtual void updateWrapper() override;
    };
}

#endif // MODELUPDATE_H
