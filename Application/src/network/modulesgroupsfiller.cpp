#include "modulesgroupsfiller.h"
#include "modulesgroups.h"
#include "modules.h"
#include <set>

ModulesGroupsFiller::ModulesGroupsFiller()
{
    connect(&registry, &Registry::decompressSuccess, this, &ModulesGroupsFiller::makeTransform);
}

inline uint32_t hash(const std::string& str) {

    uint32_t hash = 0x811c9dc5;
    uint32_t prime = 0x1000193;

    for(int i = 0; i < str.size(); ++i) {
        uint8_t value = str[i];
        hash = hash ^ value;
        hash *= prime;
    }

    return hash;
}

std::string ModulesGroupsFiller::hashKey(const QJsonObject& obj)
{
    QStringList list;
    list << ModulesGroups::parseName(obj.value("fil").toString());
    list << obj.value("lng").toString();
    list << obj.value("reg").toString();
    return list.join("").toStdString();
}

void ModulesGroupsFiller::makeTransform(const QJsonArray& source)
{
    std::map<uint32_t, ModulesGroups> modulesGroups;

    for (QJsonArray::const_iterator it = source.begin(); it != source.end(); it++)
    {
        std::pair<std::map<uint32_t, ModulesGroups>::iterator, bool> itModules;
        itModules = modulesGroups.insert({hash(hashKey(it->toObject())), ModulesGroups(it->toObject())});
        itModules.first->second.addModule(Modules(it->toObject()));
        qDebug() << itModules.first->second.m_modules[0].m_name;
        // delete *it ?
    }
    qDebug() << modulesGroups.size();
}
