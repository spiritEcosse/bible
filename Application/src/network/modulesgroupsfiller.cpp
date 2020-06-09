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
        modulesGroups[hash(hashKey(it->toObject()))] = ModulesGroups(it->toObject());
    }
    qDebug() << modulesGroups.size();

//    std::vector<ModulesGroups> target;
//    std::transform(source.begin(), source.end(), std::back_inserter(target),
//                   [](const QJsonValue& value) {
//        return ModulesGroups(value.toObject());
//    });
}
