#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QDebug>
#include "modelregistry.h"
#include "sqlite_orm.h"

ModelRegistry::ModelRegistry()
    : m_db { new db::Processor<Registry>{} }
{
}

void ModelRegistry::update(const QJsonDocument& document)
{
    struct User{
        int id;
        std::string firstName;
        std::string lastName;
        int birthDate;
        std::unique_ptr<std::string> imageUrl;
        int typeId;
    };

    struct UserType {
        int id;
        std::string name;
    };

    using namespace sqlite_orm;
    auto storage = make_storage("db.sqlite",
                                make_table("users",
                                           make_column("id", &User::id, autoincrement(), primary_key()),
                                           make_column("first_name", &User::firstName),
                                           make_column("last_name", &User::lastName),
                                           make_column("birth_date", &User::birthDate),
                                           make_column("image_url", &User::imageUrl),
                                           make_column("type_id", &User::typeId)),
                                make_table("user_types",
                                           make_column("id", &UserType::id, autoincrement(), primary_key()),
                                           make_column("name", &UserType::name, default_value("name_placeholder"))));
//    storage.insert_range

    storage.sync_schema(true);
    std::vector<User> users;

    for (int i = 0; i < 2000; i++) {
        users.push_back(User {-1, "Jonh", "Doe", 664416000, std::make_unique<std::string>("url_to_heaven"), 3 });
//        qWarning() << "insertedId << " << insertedId;
    }
    storage.insert_range(users.begin(), users.end());
    users.clear();

//    saveRegistries(getRegistries(document));
//    emit updateSuccess();
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
    std::vector<Registry>&& registries = transform(array);
    m_db->save(registries);
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

