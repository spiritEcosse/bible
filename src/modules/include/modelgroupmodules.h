#ifndef MODELGROUPMODULES_H
#define MODELGROUPMODULES_H

#include "managergroup.h"
#include "managerregistry.h"
#include "modelmodule.h"
#include "modelupdate.h"

namespace modules {

    namespace tests {
        class tst_ModelGroupModules;
    }

    class ModelGroupModules : public ModelUpdate<GroupModules> {
        Q_OBJECT
        Q_PROPERTY(bool updateCompleted READ updateCompleted NOTIFY changeUpdateCompleted)
        Q_PROPERTY(bool postEmptyModelCompleted READ postEmptyModelCompleted NOTIFY changePostEmptyModelCompleted)
        Q_PROPERTY(QString needle READ needle NOTIFY changeNeedle)
    public:
        enum GroupModulesRoles {
            TitleRole = 0,
            RegionRole = 1,
            ModulesRole = 2,
            CountModulesRole = 3,
            IdRole = 4,
            GroupIdRole = 5,
            Hidden = 6,
        };

        enum EntitySearch { GroupSearch = 0, ModuleSearch = 1 };

        ModelGroupModules();
        virtual ~ModelGroupModules();
        Q_INVOKABLE void init();
        static void registerMe();
        virtual QVariant data(const QModelIndex &index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
        Q_INVOKABLE virtual void downloadRegistry();
        Q_INVOKABLE virtual void postEmptyModel();
        Q_INVOKABLE virtual void search(const QString &needle);
        Q_INVOKABLE void getAll();
        Q_INVOKABLE bool searchByModules() const;
        Q_INVOKABLE bool searchByGroups() const;
        QString getNameJson() override;

    private:
        friend class tests::tst_ModelGroupModules;
        bool updateCompleted() const;
        bool postEmptyModelCompleted() const;
        QString needle() const;
        QString m_needle = "";
        int m_entitySearch = GroupSearch;
        bool m_updateCompleted = false;
        bool m_postEmptyModelCompleted = false;
        std::unique_ptr<ManagerGroup> m_managerGroup;
        std::unique_ptr<ManagerRegistry> m_managerRegistry;
        std::unique_ptr<ModelModule> m_modelModule;

        virtual void update();
        virtual void setFieldSearch(const QString &needle);
        virtual void doSearchByModules();
        virtual void doSearchByGroups();
    signals:
        void changeUpdateCompleted();
        void changeNeedle();
        void changePostEmptyModelCompleted();
    private slots:
        void setUpdateCompleted();
        void updateObjects();
        void setPostEmptyModelCompleted();
    };

}  // namespace modules

#endif  // MODELGROUPMODULES_H
