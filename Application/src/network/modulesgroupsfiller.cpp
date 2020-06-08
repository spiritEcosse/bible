#include "modulesgroupsfiller.h"
#include "modulesgroups.h"
#include "modules.h"
#include <set>

ModulesGroupsFiller::ModulesGroupsFiller()
{
    connect(&registry, &Registry::decompressSuccess, this, &ModulesGroupsFiller::makeTransform);
}

inline uint32_t hash_str_uint32(const std::string& str) {

    uint32_t hash = 0x811c9dc5;
    uint32_t prime = 0x1000193;

    for(int i = 0; i < str.size(); ++i) {
        uint8_t value = str[i];
        hash = hash ^ value;
        hash *= prime;
    }

    return hash;

}

void ModulesGroupsFiller::makeTransform(const QJsonArray& source)
{
    std::set<uint32_t> set;
    std::vector<ModulesGroups> groups;
    std::vector<Modules> modules;

    for (QJsonArray const_iterator = source.begin(); )
//    std::vector<ModulesGroups> target;
//    std::transform(source.begin(), source.end(), std::back_inserter(target),
//                   [](const QJsonValue& value) {
//        return ModulesGroups(value.toObject());
//    });
}
