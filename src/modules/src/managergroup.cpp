#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <algorithm>
#include <iterator>

#include "managergroup.h"

namespace modules {

    ManagerGroup::ManagerGroup(QObject *parent)
        : QObject(parent),
          m_managerRegistry { new ManagerRegistry {} },
          m_modelModule { new ModelModule {} }
    {
        connect(m_managerRegistry.get(), &ManagerRegistry::retrieveDataSuccess, this, &ManagerGroup::makeCollections);
        connect(this, &ManagerGroup::makeModulesSuccess, m_modelModule.get(), &ModelModule::update);
    }

    const QJsonArray ManagerGroup::getDownloads(const QJsonDocument& document) const
    {
        return document.object().value("downloads").toArray();
    }

    void ManagerGroup::downloadRegistry()
    {
        m_managerRegistry->download();
    }

    void ManagerGroup::makeCollections(const QJsonDocument& document)
    {
        const QJsonArray& source = getDownloads(document);

        std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual> mGMap;
        std::vector<Module> modules;

        int id = 1;
        for (auto it = source.begin(); it != source.end(); it++)
        {
            Module module {it->toObject()};
            GroupModules groupModules(it->toObject());
            const MGKey mgKey {groupModules.nameToStdString(), groupModules.languageCodeToStdString(), groupModules.regionToStdString()};
            if (mGMap.insert({mgKey, groupModules}).second)
            {
                groupModules.m_id = id++;
            }

            module.m_idGroupModules = groupModules.m_id;
            modules.push_back(module);
        }

        emit makeModulesSuccess(modules);
        transform(mGMap);
    }

    void ManagerGroup::transform(const std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual> &source)
    {
        typedef std::unordered_map<MGKey, GroupModules, MGKeyHash, MGKeyEqual>::const_iterator Iter;

        std::vector<GroupModules> target;
        std::transform(std::move_iterator<Iter>(source.begin()),
                std::move_iterator<Iter>(source.end()),
                std::back_inserter(target),
                [](std::pair<MGKey, GroupModules>&& entry)
        {
            return std::move(entry.second);
        });

        emit makeGroupModulesSuccess(target);
    }

}
