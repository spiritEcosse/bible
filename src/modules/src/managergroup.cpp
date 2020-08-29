#include "managergroup.h"
#include "module.h"
#include <QJsonArray>
#include <QJsonObject>


ManagerGroup::ManagerGroup(QObject *parent)
    : QObject(parent), m_modelRegistry { new ModelRegistry {} }
{
    connect(&*m_modelRegistry, &ModelRegistry::updateDone, this, &ManagerGroup::run);
}

void ManagerGroup::downloadRegistry()
{
//    m_registry->download();
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

void ManagerGroup::run(const QJsonArray& source)
{
    emit completed(addToCollection(source));
}
