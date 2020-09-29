#ifndef MODELREGISTRY_H
#define MODELREGISTRY_H

#include <QObject>
#include <QAbstractListModel>
#include <memory>
#include <processor.h>

#include "registry.h"

namespace TestManagerRegistry
{
   class tst_ManagerRegistry;
}

namespace TestManagerGroup
{
   class tst_ManagerGroup;
}

namespace TestModelRegistry
{
   class tst_ModelRegistry;
}

class ModelRegistry : public QAbstractListModel
{
    Q_OBJECT
public:
    ModelRegistry();
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
private:
    friend class TestManagerRegistry::tst_ManagerRegistry;
    friend class TestManagerGroup::tst_ManagerGroup;
    friend class TestModelRegistry::tst_ModelRegistry;

    std::unique_ptr<db::Processor<Registry>> m_db;
    std::vector<Registry> m_registries {
        Registry {
            "aHR0cDovL21waDQucnUvcmVnaXN0cnkuemlw",
            1,
            "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u"
        }
    };
    int index = 0;
    enum RegistryRoles
    {
        UrlRole,
        PriorityRole,
        InfoUrlRole
    };
    void addRegistry();
    const QJsonArray getRegistries(const QJsonDocument &document) const;
    bool saveRegistries(const QJsonArray &array);
signals:
    void updateSuccess();
    void registry(const Registry& registry);

public slots:
    void update(const QJsonDocument& document);
    void getRegistry();
};

#endif // MODELREGISTRY_H
