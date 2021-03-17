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
        qDebug() << rowCount();
        emit beginResetModel();

        const auto &objects = m_db->storage->get_all<GroupModules>(
                    sqlite_orm::multi_order_by(
                        sqlite_orm::order_by(&GroupModules::m_language),
                        sqlite_orm::order_by(&GroupModules::m_region),
                        sqlite_orm::order_by(&GroupModules::m_name)
                    ), sqlite_orm::limit(itemsToFetch, sqlite_orm::offset(rowCount())));
        m_objects.insert(m_objects.end(), objects.begin(), objects.end());
        emit endResetModel();
    }

    bool ModelGroupModules::canFetchMorePatch() const
    {
        qDebug() << "canFetchMore" << rowCount();
        return rowCount() < m_db->count();
    }

    void ModelGroupModules::fetchMorePatch()
    {
        qDebug() << "fetchMore" << rowCount();
        int remainder = m_db->count() - rowCount();
        itemsToFetch = qMin(50, remainder);

        updateObjects();
//        beginInsertRows(QModelIndex(), fileCount, fileCount + itemsToFetch - 1);
//        fileCount += itemsToFetch;
//        endInsertRows();
    }

    void ModelGroupModules::downloadRegistry()
    {
        m_newVersionAvailable = false;
        emit changeNewVersionAvailable();
        QTimer::singleShot(0, m_managerGroup.get(), &ManagerGroup::downloadRegistry);
    }

    QVariant ModelGroupModules
    ::data(const QModelIndex & index, int role) const {
        if (!index.isValid() || index.row() > rowCount(index)) {
            return {};
        }

//        if (!hasIndex(index.row(), index.column(), index.parent()))
//            return {};

        const auto &groupModules = m_objects.at(index.row());
        if (role == TitleRole) {
            return groupModules.titleGroup();
        }
        return {};
    }

    QHash<int, QByteArray>
    ModelGroupModules::roleNames() const {
        return {
            { TitleRole, "titleGroup" },
        };
    }

}
