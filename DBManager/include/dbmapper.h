#pragma once
#include "dbtypes.h"

namespace db
{
static const std::map<DBTables, QString> TableMapper
{
    {DBTables::Modules, "Modules"}
};

static const std::map<QString, QString> TableMapping
{
    {"Modules", "name, description"}
};

}
