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
        ModelModule();
        virtual ~ModelModule();
        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        friend class tests::tst_ModelModule;
    };

}

#endif // MODELMODULE_H
