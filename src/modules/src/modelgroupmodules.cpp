#include <QtQuick>
#include "modelgroupmodules.h"
#include <QDebug>


namespace modules {

    ModelGroupModules::ModelGroupModules()
        : m_managerGroup { new ManagerGroup {} }
    {
        m_newVersionAvailable = m_managerGroup->m_managerRegistry->hasNewRegistry();
        updateObjects();
        connect(m_managerGroup.get(), &ManagerGroup::makeGroupModulesSuccess, this, &ModelGroupModules::update);
        connect(this, &ModelGroupModules::updateDone, this, &ModelGroupModules::setUpdateCompleted);
        connect(this, &ModelGroupModules::updateDone, this, &ModelGroupModules::updateObjects);
    }

    ModelGroupModules::~ModelGroupModules() {}

    void ModelGroupModules::registerMe()
    {
        qmlRegisterType<ModelGroupModules>("bible.ModelGroupModules", 1, 0, "ModelGroupModules");
    }

    bool ModelGroupModules::newVersionAvailable() const
    {
        return m_newVersionAvailable;
    }

    bool ModelGroupModules::updateCompleted() const
    {
        return m_updateCompleted;
    }

    void ModelGroupModules::setUpdateCompleted()
    {
        m_updateCompleted = true;
        emit changeUpdateCompleted();
    }

    void ModelGroupModules::updateObjects()
    {
        beginResetModel();
        m_objects = m_db->storage->get_all<GroupModules>(
                    sqlite_orm::multi_order_by(
                        sqlite_orm::order_by(&GroupModules::m_region),
                        sqlite_orm::order_by(&GroupModules::m_language),
                        sqlite_orm::order_by(&GroupModules::m_name)
                    ));
        endResetModel();
    }

    bool ModelGroupModules::canFetchMore(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return false;
        return (objectsCount < static_cast<int>(m_objects.size()));
    }

    void ModelGroupModules::fetchMore(const QModelIndex &parent)
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

    void ModelGroupModules::downloadRegistry()
    {
        m_newVersionAvailable = false;
        emit changeNewVersionAvailable();
        QTimer::singleShot(0, m_managerGroup.get(), &ManagerGroup::downloadRegistry);
    }

    QVariant ModelGroupModules
    ::data(const QModelIndex & index, int role) const {
        QVariant data {};

        if (!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &groupModules = m_objects.at(index.row());

        switch(role) {
            case TitleRole :
                data = std::move(groupModules.titleGroup());
                break;
            case RegionRole :
                data = std::move(groupModules.region());
                break;
        }

        return data;
    }

    QHash<int, QByteArray>
    ModelGroupModules::roleNames() const {
        return {
            { TitleRole, "titleGroup" },
            { RegionRole, "region" },
        };
    }

}
