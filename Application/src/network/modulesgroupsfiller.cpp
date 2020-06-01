#include "modulesgroupsfiller.h"
#include "modulesgroups.h"

ModulesGroupsFiller::ModulesGroupsFiller()
{
    connect(&registry, &Registry::decompressSuccess, this, &ModulesGroupsFiller::makeTransform);
}

void ModulesGroupsFiller::makeTransform(const QJsonArray& source)
{
    std::vector<ModulesGroups> target;
    std::transform(source.begin(), source.end(), std::back_inserter(target),
                   [](const QJsonValue& value) {
        return ModulesGroups(value.toObject());
    });
}
