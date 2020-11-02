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
    public:
        ModelGroupModules();
        virtual ~ModelGroupModules();
        static void registerMe(const std::string& moduleName);
        virtual QVariant data(const QModelIndex &index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        friend class tests::tst_ModelGroupModules;

        std::unique_ptr<ManagerGroup> m_managerGroup;
    };

}

#endif // MODELGROUPMODULES_H
