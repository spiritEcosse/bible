#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "managergroup.h"

namespace modules {

    ManagerGroup::ManagerGroup(QObject *parent)
        : QObject(parent),
          m_managerRegistry { new ManagerRegistry {} }
    {
        connect(m_managerRegistry.get(), &ManagerRegistry::retrieveDataSuccess, this, &ManagerGroup::makeGroup);
    }

    const QJsonArray ManagerGroup::getDownloads(const QJsonDocument& document) const
    {
        return document.object().value("downloads").toArray();
    }

    void ManagerGroup::downloadRegistry()
    {
        m_managerRegistry->download();
    }

    std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual>
    ManagerGroup::addToCollection(const QJsonArray& source)
    {
        std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual> mGMap;
        std::vector<Module> modules;

        for (QJsonArray::const_iterator it = source.begin(); it != source.end(); it++)
        {
            GroupModules groupModules(it->toObject());
            const MGKey mgKey {groupModules.nameToStdString(), groupModules.languageCodeToStdString(), groupModules.regionToStdString()};
            auto itMGMap = mGMap.insert({mgKey, groupModules});
            Module module {it->toObject()};
            module.m_idGroupModules = groupModules.m_id;
//            itMGMap.first->second
            modules.push_back(module);
        }

        return mGMap;
    }

    void ManagerGroup::makeGroup(const QJsonDocument& document)
    {
//        addToCollection(getDownloads(document));
        std::vector<GroupModules> groupModules;
        std::vector<Module> modules;
        emit makeGroupModulesSuccess(groupModules);
        emit makeModulesSuccess(modules);
    }

}
