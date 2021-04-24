#ifndef MANAGERGROUP_H
#define MANAGERGROUP_H

#include <QObject>

#include "managerregistry.h"
#include "groupmodules.h"
#include "modelmodule.h"

namespace modules {

    namespace tests
    {
       class tst_ManagerGroup;
       class tst_ModelGroupModules;
    }

    class ManagerGroup : public QObject
    {
        Q_OBJECT
    public:
        ManagerGroup(QObject *parent = nullptr);
        virtual ~ManagerGroup() {}
    public slots:
        void downloadRegistry();

    signals:
        void makeModulesSuccess(const std::vector<Module>& modules);
        void error(const QString& error);
        void makeGroupModulesSuccess();

    private:
        friend class tests::tst_ManagerGroup;
        friend class tests::tst_ModelGroupModules;
        friend class ModelGroupModules;
        GroupModulesMap m_objects;

        std::unique_ptr<ManagerRegistry> m_managerRegistry;
        std::unique_ptr<ModelModule> m_modelModule;
        void addToCollection(const QJsonArray& object);
        virtual const QJsonArray getDownloads(const QJsonDocument& document) const;
    private slots:
        void makeCollections(const QJsonDocument& document);
    };

}

#endif // MANAGERGROUP_H
