#include "modelhost.h"

namespace modules {

    using namespace sqlite_orm;

    ModelHost::ModelHost()
    {
        connect(this, &ModelHost::transformSuccess, this, &ModelHost::updateObjectsFromJson);
    }

    ModelHost::~ModelHost() {}

    QUrl ModelHost::data(int index, int role) const
    {
        QUrl url;
        if (index > static_cast<int>(m_objects.size())) {
            return url;
        }

        const auto &object = m_objects.at(index);

        switch (role) {
            case HostRoles::PathRole:
                url = object->pathToQUrl();
                break;
        }

        return url;
    }

    // overridden from qt

    QVariant ModelHost::data(const QModelIndex &index, int role) const {}

    QHash<int, QByteArray> ModelHost::roleNames() const {}

}
