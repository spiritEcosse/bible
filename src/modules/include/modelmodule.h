#ifndef MODELMODULE_H
#define MODELMODULE_H

#include "modelupdate.h"
#include "module.h"


namespace modules {

    class ModelModule : public ModelUpdate<Module>
    {
        Q_OBJECT
    public:
        ModelModule();
        virtual ~ModelModule();

        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
    };

}

#endif // MODELMODULE_H
