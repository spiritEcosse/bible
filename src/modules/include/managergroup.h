#ifndef MANAGERGROUP_H
#define MANAGERGROUP_H

#include <QObject>

#include "groupmodules.h"
#include "module.h"

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
    signals:
        void makeModulesSuccess(const std::vector<Module>& modules);
        void error(const QString& error);
        void makeGroupModulesSuccess();

    private:
        friend class tests::tst_ManagerGroup;
        friend class tests::tst_ModelGroupModules;
        friend class ModelGroupModules;
        GroupModulesMap m_objects;

        void addToCollection(const QJsonArray& object);
        virtual const QJsonArray getDownloads(const QJsonDocument& document) const;
    private slots:
        void makeCollections(const QJsonDocument& document);
    };

}

#endif // MANAGERGROUP_H
