#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QDebug>
#include "modelregistry.h"

ModelRegistry::ModelRegistry()
    : m_db { db::Db::getInstance() }
{
}

void ModelRegistry::update(const QJsonDocument& document)
{
    saveRegistries(getRegistries(document));
    emit updateSuccess();
}

const QJsonArray ModelRegistry::getRegistries(const QJsonDocument &document) const
{
    return document.object().value("registries").toArray();
}

std::vector<Registry> transform(const QJsonArray &source)
{
    std::vector<Registry> target;
    std::transform(source.begin(), source.end(), std::back_inserter(target),
                   [](const QJsonValue& entry)
    {
        return Registry { entry.toObject() };
    });
    return target;
}

void ModelRegistry::saveRegistries(const QJsonArray &array)
{
    const std::vector<Registry>& registries = transform(array);
    m_db->storage->insert_range(registries.begin(), registries.end());
    m_registries = registries;
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

void ModelRegistry::getRegistry()
{
    try {
        emit registry(m_registries.at(index));
        ++index;
    } catch (std::out_of_range) {

    }
}

void ModelRegistry::addRegistry()
{

}

