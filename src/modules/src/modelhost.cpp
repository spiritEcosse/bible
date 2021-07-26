#include "modelhost.h"
#include <mutex>
#include <QDebug>

namespace modules {

    using namespace sqlite_orm;

    std::vector<HostUnique> ModelHost::objectsStatic;

    ModelHost::ModelHost()
    {
        connect(this, &ModelHost::transformSuccess, this, &ModelHost::updateObjectsFromJson);
    }

    ModelHost::~ModelHost() {}

    QString ModelHost::data(int index, int role) const
    {
        QString str;
        if (index > static_cast<int>(m_objects.size())) {
            return str;
        }

        const auto &object = objectsStatic.at(index);

        switch (role) {
            case HostRoles::PathRole:
                str = object->pathToQString();
                break;
        }

        return str;
    }

    void ModelHost::populateStaticObjects()
    {
        objectsStatic = m_db->storage->get_all_pointer<Host>(
                    multi_order_by(
                        order_by(&Host::m_weight).desc(),
                        order_by(&Host::m_priority)
                    ));
    }

    // overridden from qt

    QVariant ModelHost::data(const QModelIndex &index, int role) const {}

    QHash<int, QByteArray> ModelHost::roleNames() const {}

}
