#include "modulesgroupsfiller.h"
#include "modulesgroups.h"
#include "modules.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

ModulesGroupsFiller::ModulesGroupsFiller()
{
//    connect(&registry, &Registry::decompressSuccess, this, &ModulesGroupsFiller::makeTransform);
}

std::unordered_map<MGKey, ModulesGroups, MGKeyHash, MGKeyEqual> ModulesGroupsFiller::makeTransform(const QJsonArray& source)
{
    std::unordered_map<MGKey, ModulesGroups, MGKeyHash, MGKeyEqual> mGMap;

    for (QJsonArray::const_iterator it = source.begin(); it != source.end(); it++)
    {
        ModulesGroups mg(it->toObject());
        const MGKey mgKey {mg.nameToStdString(), mg.languageCodeToStdString(), mg.regionToStdString()};
        auto itMGMap = mGMap.insert({mgKey, mg});
        itMGMap.first->second.addModule(Modules(it->toObject()));
        // delete *it ?
    }

    return mGMap;
}
