#pragma once
#include <QVariantList>

namespace DBTypes
{
enum class DBResult
{
    OK,
    FAIL
};

enum class DBTables
{
    Modules,
    ModulesGroups
};

enum class DBState
{
    OK,
    ERROR_NO_DRIVER,
    ERROR_WORKSPACE,
    ERROR_TABLES,
    ERROR_OPENING,
};

using DBEntry = QVariantList;
using DBIndex = int;

}
