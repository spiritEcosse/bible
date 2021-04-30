#include "modelregistry.h"
#include <QtDebug>

namespace modules {

    ModelRegistry::ModelRegistry()
    {
        connect(this, &ModelRegistry::transformSuccess, this, &ModelRegistry::updateObjectsFromJson);
        m_objects.push_back(baseRegistry());
    }

    ModelRegistry::~ModelRegistry() {}

    std::unique_ptr<Registry> ModelRegistry::baseRegistry() const
    {
        return std::make_unique<Registry>(
            "aHR0cDovL21waDQucnUvcmVnaXN0cnkuemlw",
            "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u"
        );
    }

    QUrl ModelRegistry::data(int index, int role) const
    {
        QUrl url;
        if (index > static_cast<int>(m_objects.size())) {
            return url;
        }

        const auto &registry = m_objects.at(index);

        switch (role) {
            case RegistryRoles::UrlRole:
                url = registry->urlToQUrl();
                break;
            case RegistryRoles::InfoUrlRole:
                url = registry->infoUrlToQUrl();
                break;
        }

        return url;
    }

    // db queries

    bool ModelRegistry::setRegistries()
    {
        m_objects = m_db->storage->get_all_pointer<Registry>(
                    sqlite_orm::order_by(&Registry::m_priority));
        return !m_objects.empty();
    }

    // overridden from qt

    QVariant ModelRegistry::data(const QModelIndex& index, int role) const
    {
        QVariant data {};

        if (!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &registry = m_objects.at(index.row());

        switch (role) {
            case RegistryRoles::UrlRole:
                data = std::move(registry->m_url);
                break;
            case RegistryRoles::PriorityRole:
                data = std::move(registry->m_priority);
                break;
            case RegistryRoles::InfoUrlRole:
                return QVariant::fromValue(registry->m_infoUrl);
                break;
        }

        return data;
    }

    QHash<int, QByteArray> ModelRegistry::roleNames() const
    {
        return {
            { UrlRole, "url" },
            { PriorityRole, "priority" },
            { InfoUrlRole, "info_url" },
        };
    }

}
