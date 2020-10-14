#ifndef MODELREGISTRY_H
#define MODELREGISTRY_H

#include <QObject>
#include <QAbstractListModel>
#include <memory>
#include "db.h"

#include "registry.h"

namespace modules {

    namespace tests
    {
       class tst_ManagerRegistry;
       class tst_ManagerGroup;
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
        enum RegistryRoles
        {
            UrlRole,
            PriorityRole,
            InfoUrlRole
        };

        friend class tests::tst_ManagerRegistry;
        friend class tests::tst_ManagerGroup;
        friend class tests::tst_ModelRegistry;

        std::shared_ptr<db::Db> m_db;
        std::vector<Registry> m_registries {
            Registry {
                "aHR0cDovL21waDQucnUvcmVnaXN0cnkuemlw",
                "aHR0cDovL21waDQucnUvcmVnaXN0cnlfaW5mby5qc29u"
            }
        };
        int index = 0;
        void deleteAllRegistries() const;
        bool setRegistries();
        void saveRegistries(const std::vector<Registry>& registries) const;
        const QJsonArray getRegistries(const QJsonDocument &document) const;
    signals:
        void updateDone();
        void error(const QString& error);
        void registry(const Registry& registry);

    public slots:
        void update(const QJsonDocument& document);
        void getRegistry();
    };

}

#endif // MODELREGISTRY_H
