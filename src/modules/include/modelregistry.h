#ifndef MODELREGISTRY_H
#define MODELREGISTRY_H

#include <QObject>
#include <QAbstractListModel>
#include <memory>

#include "registry.h"

#define FILE_BASE_PATTERN "aHR0cDovL21waDQucnUvcmVnaXN0cn"


namespace TestManagerRegistry
{
   class tst_ManagerRegistry;
}

namespace TestManagerGroup
{
   class tst_ManagerGroup;
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
signals:
    void updateSuccess();
    void registry(const Registry& registry);

public slots:
    void update(const QJsonArray& array);
    void getRegistry();
};

#endif // MODELREGISTRY_H