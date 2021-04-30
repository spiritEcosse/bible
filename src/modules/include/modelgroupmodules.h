#ifndef MODELGROUPMODULES_H
#define MODELGROUPMODULES_H

#include "modelupdate.h"
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
            TitleRole = 0,
            RegionRole = 1,
            ModulesRole = 2,
            CountModulesRole = 3
        };
        ModelGroupModules();
        virtual ~ModelGroupModules();
        static void registerMe();
        virtual QVariant data(const QModelIndex &index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
        Q_INVOKABLE virtual void downloadRegistry();
        Q_INVOKABLE virtual void search(const QString& needle);
        Q_INVOKABLE void getAll();
        inline const QString getNameJson() override { return QString("downloads"); };
    private:
        friend class tests::tst_ModelGroupModules;
        bool newVersionAvailable() const;
        bool updateCompleted() const;
        bool m_newVersionAvailable = false;
        bool m_updateCompleted = false;
        std::unique_ptr<ManagerGroup> m_managerGroup;
        virtual void update();
    signals:
        void changeNewVersionAvailable();
        void changeUpdateCompleted();
    private slots:
        void setUpdateCompleted();
        void updateObjects();
    };

}

#endif // MODELGROUPMODULES_H
