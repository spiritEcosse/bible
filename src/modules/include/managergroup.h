#ifndef MANAGERGROUP_H
#define MANAGERGROUP_H

#include <QObject>
#include <unordered_map>

#include "managerregistry.h"
#include "groupmodules.h"

namespace modules {

    // WARNING: replace this three on one class
    struct MGKey {
        std::string name;
        std::string language;
        std::string region;
    };

    struct MGKeyHash {
        std::size_t operator()(const MGKey& mgKey) const
        {
            using namespace std;
            return ((hash<string>()(mgKey.name)
                     ^ (hash<string>()(mgKey.language) << 1)) >> 1)
                     ^ (hash<string>()(mgKey.region) << 1);
        }
    };

    struct MGKeyEqual {
        bool operator()(const MGKey& lhs, const MGKey& rhs) const
        {
            return lhs.name == rhs.name && lhs.language == rhs.language && lhs.region == rhs.region;
        }
    };

    namespace tests
    {
       class tst_ManagerGroup;
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
        void makeGroupModulesSuccess(const std::vector<GroupModules>& groupModules);
        void makeModulesSuccess(const std::vector<Module>& modules);

    private:
        friend class tests::tst_ManagerGroup;

        std::unique_ptr<ManagerRegistry> m_managerRegistry;
        std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual> addToCollection(const QJsonArray& object);
        virtual const QJsonArray getDownloads(const QJsonDocument& document) const;
    private slots:
        void makeGroup(const QJsonDocument& document);
    };

}

#endif // MANAGERGROUP_H
