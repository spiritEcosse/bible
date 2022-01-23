#include "modelhost.h"
#include <mutex>
#include <QDebug>

namespace modules {

    using namespace sqlite_orm;

    SingletonModelHost::SingletonModelHost()
    {
        populateObjects();
        qDebug() << "SingletonModelHost";
    }

    void SingletonModelHost::populateObjects() {
        m_objects = m_db->storage->get_all_pointer<Host>(
                    multi_order_by(
                        order_by(&Host::m_weight).desc(),
                        order_by(&Host::m_priority)
                    ));
        m_objects.insert(m_objects.begin(), ModelHost::baseHost());
    }

    ModelHost::ModelHost()
    {
        connect(this, &ModelHost::transformSuccess, this, &ModelHost::updateObjectsFromJson);
        m_objects.push_back(baseHost());
    }

    ModelHost::~ModelHost() {}

    std::unique_ptr<Host> ModelHost::baseHost()
    {
        return std::make_unique<Host>(
            "p4",
            "aHR0cDovL21waDQucnUvbS8lcy56aXA="
        );
    }

    QString ModelHost::getUrl(int index) const
    {
        return index > 0 ? SingletonModelHost::getInstance().m_objects[index]->pathToQString() :
                           m_objects[0]->pathToQString();
    }

    bool ModelHost::existsIndex(int index) const
    {
        return index < static_cast<int>(SingletonModelHost::getInstance().m_objects.size());
    }

    // overridden from qt

    QVariant ModelHost::data(const QModelIndex &index, int role) const {}

    QHash<int, QByteArray> ModelHost::roleNames() const {}

}
