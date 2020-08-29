#ifndef MODELREGISTRY_H
#define MODELREGISTRY_H

#include <QObject>
#include <QAbstractListModel>
#include "registry.h"


class ModelRegistry : public QAbstractListModel
{
    Q_OBJECT
public:
    ModelRegistry();
    void update();
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;
signals:
    std::vector<Registry> m_registries;
    void updateDone();

    enum RegistryRoles
    {
        UrlRole,
        PriorityRole,
        InfoUrlRole
    };
};

#endif // MODELREGISTRY_H
