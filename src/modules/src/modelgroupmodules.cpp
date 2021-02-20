#include <QtQuick>
#include "modelgroupmodules.h"
#include <QDebug>


namespace modules {

    ModelGroupModules::ModelGroupModules()
        : m_managerGroup { new ManagerGroup {} }
    {
        m_newVersionAvailable = true;
//        m_newVersionAvailable = m_managerGroup->m_managerRegistry->hasNewRegistry();
        connect(m_managerGroup.get(), &ManagerGroup::makeGroupModulesSuccess, this, &ModelGroupModules::update);
        connect(this, &ModelGroupModules::updateDone, this, &ModelGroupModules::setUpdateCompleted);
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

    void ModelGroupModules::downloadRegistry()
    {
        QTimer::singleShot(0, m_managerGroup.get(), &ManagerGroup::downloadRegistry);
        m_newVersionAvailable = false;
        emit changeNewVersionAvailable();
    }
	
    QVariant ModelGroupModules
    ::data(const QModelIndex &index, int role) const
    {
    //    if (role < Qt::UserRole) {
    //        return QSqlTableModel::data(index, role);
    //    }

    //    const QSqlRecord sqlRecord = record(index.row());
    //    return sqlRecord.value(role - Qt::UserRole);
    }

    QHash<int, QByteArray>
    ModelGroupModules::roleNames() const {
        QHash<int, QByteArray> names;
        names[Qt::UserRole] = "title";
        return names;
    }

}
