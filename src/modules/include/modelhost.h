#ifndef MODELHOST_H
#define MODELHOST_H

#include "modelupdate.h"
#include <QObject>

namespace modules {

    class ModelHost : public ModelUpdate<Host>
    {
        Q_OBJECT
    public:
        enum HostRoles
        {
            PathRole = 0,
        };

        ModelHost();
        ~ModelHost();

        virtual QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
        virtual QUrl data(int index, int role) const;
        inline const QString getNameJson() override { return QString("hosts"); };
    };
    using HostShared = decltype(std::shared_ptr<Host>());
    using HostUnique = decltype(std::unique_ptr<Host>());
}

#endif // MODELHOST_H
