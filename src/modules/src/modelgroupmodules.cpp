#include <QtQuick>
#include "modelgroupmodules.h"

namespace modules {

    ModelGroupModules::ModelGroupModules()
        : m_managerGroup { new ManagerGroup {} }
    {
        connect(m_managerGroup.get(), &ManagerGroup::makeGroupModulesSuccess, this, &ModelGroupModules::update);
    }

    ModelGroupModules::~ModelGroupModules() {}

    void ModelGroupModules::registerMe()
    {
        qmlRegisterType<ModelGroupModules>("bible.ModelGroupModules", 1, 0, "ModelGroupModules");
    }

    void ModelGroupModules::downloadRegistry() const
    {
        m_managerGroup->downloadRegistry();
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
