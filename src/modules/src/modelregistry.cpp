#include <QJsonArray>

#include "modelregistry.h"


ModelRegistry::ModelRegistry()
{
}

void ModelRegistry::update(const QJsonArray& array)
{
    emit updateSuccess();
}

int ModelRegistry::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(m_registries.size());
}

QVariant ModelRegistry::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() > rowCount(index)) {
        return {};
    }

    const Registry& registry = m_registries.at(index.row());

    switch (role) {
        case RegistryRoles::UrlRole: {
            return QVariant::fromValue(registry.url());
        }
        case RegistryRoles::PriorityRole: {
            return QVariant::fromValue(registry.priority());
        }
        case RegistryRoles::InfoUrlRole: {
            return QVariant::fromValue(registry.infoUrl());
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

std::vector<Registry>::const_iterator ModelRegistry::getRegistry()
{
    return m_registriesIter++;
}
