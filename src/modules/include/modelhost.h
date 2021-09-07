#ifndef MODELHOST_H
#define MODELHOST_H

#include "modelupdate.h"
#include <QObject>

namespace netmanager {
    namespace tests {
        class tst_QuickDownload;
    }
}

namespace modules {

    namespace tests {
        class tst_ManagerRegistry;
        class tst_ModelHost;
    }

    using HostShared = decltype(std::shared_ptr<Host>());
    using HostUnique = decltype(std::unique_ptr<Host>());

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
        virtual QString data(int index, int role) const;
        inline const QString getNameJson() override { return QString("hosts"); };
        void populateStaticObjects();
    private:
        friend class tests::tst_ModelHost;
        friend class netmanager::tests::tst_QuickDownload;
        static std::vector<HostUnique> objectsStatic;
        HostUnique baseHost() const;
    };

}

#endif // MODELHOST_H
