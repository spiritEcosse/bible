#ifndef MODELHOST_H
#define MODELHOST_H

#include "modelupdate.h"
#include <QObject>

namespace netmanager {
    namespace tests {
        class tst_QuickDownload;
    }
}  // namespace netmanager

namespace modules {

    namespace tests {
        class tst_ManagerRegistry;
        class tst_ModelHost;
        class tst_ModelModule;
    }  // namespace tests

    using HostShared = decltype(std::shared_ptr<Host>());
    using HostUnique = decltype(std::unique_ptr<Host>());

    class SingletonModelHost : public ModelUpdate<Host> {
    public:
        static SingletonModelHost &getInstance() {
            static SingletonModelHost instance;
            return instance;
        }

        virtual QVariant data(const QModelIndex &index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;

        inline QString getNameJson() override {
            return {};
        };

    private:
        SingletonModelHost();
        ~SingletonModelHost() = default;
        SingletonModelHost(const SingletonModelHost &) = delete;
        SingletonModelHost &operator=(const SingletonModelHost &) = delete;
        virtual void populateObjects();
    };

    class ModelHost : public ModelUpdate<Host> {
        Q_OBJECT
    public:
        ModelHost();
        ~ModelHost();

        enum HostRoles {
            PathRole = 0,
        };

        virtual QVariant data(const QModelIndex &index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
        virtual QString getUrl(int index) const;
        virtual bool existsIndex(int index) const;

        inline QString getNameJson() override {
            return QString("hosts");
        };

        static HostUnique baseHost();

    private:
        friend class tests::tst_ModelHost;
        friend class tests::tst_ModelModule;
        friend class netmanager::tests::tst_QuickDownload;
    };

}  // namespace modules

#endif  // MODELHOST_H
