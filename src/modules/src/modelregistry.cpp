#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

#include "modelregistry.h"

namespace modules {

    ModelRegistry::ModelRegistry()
    {
        m_objects = {
            Registry {
                "aHR0cDovL21waDQucnUvcmVnaXN0cnkuemlw",
                "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u"
            }
        };
    }

    ModelRegistry::~ModelRegistry() {}

    void ModelRegistry::getRegistry()
    {
        try {
            emit registry(m_objects.at(index));
            ++index;
        } catch (std::out_of_range) {
            if (setRegistries()) {
                getRegistry();
            } else {
                emit error("An error occured, please try in time.");
            }
        }
    }

    // db queries

    bool ModelRegistry::setRegistries()
    {
        auto registries = m_db->storage->get_all<Registry>(
                    sqlite_orm::order_by(&Registry::m_priority));


        if (!registries.empty()) {
            m_objects = registries;
        }
        return !registries.empty();
    }

    // overridden from qt

    QVariant ModelRegistry::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid() || index.row() > rowCount(index)) {
            return {};
        }

        const Registry& registry = m_objects.at(index.row());

        switch (role) {
            case RegistryRoles::UrlRole: {
                return QVariant::fromValue(registry.m_url);
            }
            case RegistryRoles::PriorityRole: {
                return QVariant::fromValue(registry.m_priority);
            }
            case RegistryRoles::InfoUrlRole: {
                return QVariant::fromValue(registry.m_infoUrl);
            }
            default: {
                return {};
            }
        }
    }

    QHash<int, QByteArray> ModelRegistry::roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[RegistryRoles::UrlRole] = "url";
        roles[RegistryRoles::PriorityRole] = "priority";
        roles[RegistryRoles::InfoUrlRole] = "info_url";

        return roles;
    }

}
