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

class ModelRegistry : public QAbstractListModel
{
    Q_OBJECT
public:
    ModelRegistry();
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    std::vector<Registry>::const_iterator getRegistry();
private:
    friend class TestManagerRegistry::tst_ManagerRegistry;
    std::vector<Registry>::iterator m_registriesIter = m_registries.begin();
    std::vector<Registry> m_registries {
        Registry {
            QString("%1kuemlw").arg(FILE_BASE_PATTERN),
            1,
            QString("%1lfaW5mby5qc29u").arg(FILE_BASE_PATTERN)
        }
    };
    enum RegistryRoles
    {
        UrlRole,
        PriorityRole,
        InfoUrlRole
    };
signals:
    void updateSuccess();

public slots:
    void update(const QJsonArray& array);
};

#endif // MODELREGISTRY_H
