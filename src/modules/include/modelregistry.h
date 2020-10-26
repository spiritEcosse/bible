#ifndef MODELREGISTRY_H
#define MODELREGISTRY_H

#include <QObject>
#include <memory>
#include "db.h"
#include "modelupdate.h"

#include "registry.h"

namespace modules {

    namespace tests
    {
       class tst_ManagerRegistry;
       class tst_ManagerGroup;
       class tst_ModelRegistry;
    }

    class ModelRegistry : public ModelUpdate<Registry>
    {
        Q_OBJECT
    public:
        ModelRegistry();
        ~ModelRegistry();
        int rowCount(const QModelIndex& parent = {}) const override;
        QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;
    private:
        enum RegistryRoles
        {
            UrlRole,
            PriorityRole,
            InfoUrlRole
        };

        friend class tests::tst_ManagerRegistry;
        friend class tests::tst_ManagerGroup;
        friend class tests::tst_ModelRegistry;

        int index = 0;
        bool setRegistries();
        const QJsonArray getRegistries(const QJsonDocument &document) const;
    signals:
        void registry(const Registry& registry);

    public slots:
        void getRegistry();
    };

}

#endif // MODELREGISTRY_H
