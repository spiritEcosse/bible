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

        std::vector<Module> modules;

        try {
            int id = 1;
            for (auto it = source.begin(); it != source.end(); it++)
            {
                Module module (it->toObject());
                GroupModulesUnique groupModules = std::make_unique<GroupModules>(it->toObject());
                const MGKey mgKey {
                    groupModules->nameToStdString(),
                    groupModules->languageCodeToStdString(),
                    groupModules->regionToStdString()
                };
                groupModules->m_groupId = id;
                const auto &ob = m_objects.insert({mgKey, std::move(groupModules)});
                if (ob.second)
                    id++;
                module.m_idGroupModules = ob.first->second->m_groupId;
                ob.first->second->m_countModules++;
                modules.push_back(std::move(module));
            }

            emit makeModulesSuccess(modules);
            emit makeGroupModulesSuccess();
        } catch(const ModuleInvalidData& e) {
            qInfo() << e.what();
            emit error("An error occured, please try in time.");
        }
    }
}
