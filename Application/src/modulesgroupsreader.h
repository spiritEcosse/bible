#pragma once
#include <vector>
#include <memory>
#include "modulesgroups.h"

namespace db
{
class Processor;
}

class ModulesGroupsReader
{
public:
    ModulesGroupsReader();
    std::pair<bool,
              std::vector<ModulesGroups>> requestModulesGroupsBrowse();
    ~ModulesGroupsReader();

private:
    std::unique_ptr<db::Processor> m_dbProcessor;
};
