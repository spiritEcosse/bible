#ifndef MODELREGISTRY_H
#define MODELREGISTRY_H

#include "modelupdate.h"

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
        enum RegistryRoles
        {
            UrlRole = 0,
            PriorityRole = 1,
            InfoUrlRole = 2
        };

        ModelRegistry();
        ~ModelRegistry();
        virtual QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
        virtual QUrl data(int index, int role) const;
        bool setRegistries();
        inline const QString getNameJson() override { return QString("registries"); };
    private:
        friend class tests::tst_ManagerRegistry;
        friend class tests::tst_ManagerGroup;
        friend class tests::tst_ModelRegistry;

        RegistryUnique baseRegistry() const;
    };

}

#endif // MODELREGISTRY_H
