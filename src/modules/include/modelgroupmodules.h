#ifndef MODELGROUPMODULES_H
#define MODELGROUPMODULES_H

#include "modelupdate.h"
#include "groupmodules.h"
#include "managergroup.h"


namespace modules {

    namespace tests
    {
       class tst_ModelGroupModules;
    }

    class ModelGroupModules : public ModelUpdate<GroupModules>
    {
        Q_OBJECT
        Q_PROPERTY(bool newVersionAvailable READ newVersionAvailable NOTIFY changeNewVersionAvailable)
        Q_PROPERTY(bool updateCompleted READ updateCompleted NOTIFY changeUpdateCompleted)
    public:
        enum GroupModulesRoles {
            TitleRole = 0
        };
        ModelGroupModules();
        virtual ~ModelGroupModules();
        static void registerMe();
        virtual QVariant data(const QModelIndex &index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
        Q_INVOKABLE virtual void downloadRegistry();
    protected:
        Q_INVOKABLE bool canFetchMorePatch() const;
        Q_INVOKABLE void fetchMorePatch();
    private:
        int itemsToFetch = 50;
        friend class tests::tst_ModelGroupModules;
        bool newVersionAvailable() const;
        bool updateCompleted() const;
        bool m_newVersionAvailable = false;
        bool m_updateCompleted = false;
        std::unique_ptr<ManagerGroup> m_managerGroup;
    signals:
        void changeNewVersionAvailable();
        void changeUpdateCompleted();
    private slots:
        void setUpdateCompleted();
        void updateObjects();
    };

}

#endif // MODELGROUPMODULES_H
