#pragma once
#include "dbtypes.h"

namespace db
{
static const std::map<DBTables, QString> TableMapper
{
    {DBTables::Modules, "modules"},
    {DBTables::ModulesGroups, "modules_groups"},
};

static const std::map<QString, QString> TableMapping
{
    {"Modules", "name, description"},
    {"ModulesGroups", "language, type, region"},
};

}
