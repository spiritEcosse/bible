#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "managergroup.h"
#include "module.h"

namespace modules {

    ManagerGroup::ManagerGroup(QObject *parent)
        : QObject(parent), m_managerRegistry { new ManagerRegistry {} }
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

        for (QJsonArray::const_iterator it = source.begin(); it != source.end(); it++)
        {
            GroupModules mg(it->toObject());
            const MGKey mgKey {mg.nameToStdString(), mg.languageCodeToStdString(), mg.regionToStdString()};
            auto itMGMap = mGMap.insert({mgKey, mg});
            itMGMap.first->second.addModule(Module(it->toObject()));
            // delete *it ? // WARNING : need delete data where indicated it (it->toObject())
        }

        return mGMap;
    }

    void ManagerGroup::makeGroup(const QJsonDocument& document)
    {
        emit completed(addToCollection(getDownloads(document)));
    }

}
