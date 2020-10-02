#pragma once
#include "dbtypes.h"

using namespace DBTypes;

namespace db
{
static const std::map<DBTables, std::string> tableMapper
{
    {DBTables::Modules, "modules"},
    {DBTables::ModulesGroups, "modules_groups"},
};

static const std::map<std::string, std::string> tablesMapping
{
    {"Modules", "name, description"},
    {"modules_groups", "language, name, region"},
};

}
