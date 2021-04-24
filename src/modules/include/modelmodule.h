#ifndef MODELMODULE_H
#define MODELMODULE_H

#include "modelupdate.h"
#include "module.h"


namespace modules {

    namespace tests
    {
       class tst_ModelModule;
    }

    class ModelModule : public ModelUpdate<Module>
    {
        Q_OBJECT
    public:
        enum ModuleRoles {
            Name = 0,
            Description = 1,
            Abbreviation = 2,
            Size = 3,
            LanguageShow = 4,
            Information = 5,
            Comment = 6,
            Copyright = 7,
            DateUpdate = 8,
            Hidden = 9,
            DefaultDownload = 10,
            Id = 11
        };

        ModelModule();
        ModelModule(int idGroupModules);
        virtual ~ModelModule();
        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
        void updateObjects();
    private:
        friend class tests::tst_ModelModule;
        int m_idGroupModules = 0;
    };

}

#endif // MODELMODULE_H
