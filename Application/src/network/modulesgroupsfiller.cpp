#include "modulesgroupsfiller.h"
#include "modulesgroups.h"
#include "modules.h"
#include <QJsonArray>
#include <QJsonObject>

ModulesGroupsFiller::ModulesGroupsFiller()
{
    connect(&registry, &Registry::decompressSuccess,
            this, &ModulesGroupsFiller::run);
}

void ModulesGroupsFiller::downloadRegistry()
{
    QTimer::singleShot(0, &registry, &Registry::tryDownload);
}

std::unordered_map<MGKey, ModulesGroups, MGKeyHash, MGKeyEqual>
ModulesGroupsFiller::fill(const QJsonArray& source)
{
    std::unordered_map<MGKey, ModulesGroups, MGKeyHash, MGKeyEqual> mGMap;

    for (QJsonArray::const_iterator it = source.begin(); it != source.end(); it++)
    {
        ModulesGroups mg(it->toObject());
        const MGKey mgKey {mg.nameToStdString(), mg.languageCodeToStdString(), mg.regionToStdString()};
        auto itMGMap = mGMap.insert({mgKey, mg});
        itMGMap.first->second.addModule(Modules(it->toObject()));
        // delete *it ? // WARNING : need delete data where indicated it (it->toObject())
    }

    return mGMap;
}

void ModulesGroupsFiller::run(const QJsonArray& source)
{
    emit completed(fill(source));
}
