#include "modulesgroupsreader.h"
#include "Processor.h"
#include "dbtypes.h"
#include <iterator>

using namespace DBTypes;

ModulesGroupsReader::ModulesGroupsReader()
    : m_dbProcessor {new db::Processor {}}
{
}

ModulesGroupsReader::~ModulesGroupsReader()
{
}

std::vector<ModulesGroups> transform(const std::vector<DBEntry>& source)
{
    std::vector<ModulesGroups> target;
    std::transform(source.begin(), source.end(), std::back_inserter(target),
                   [](const DBEntry& entry) {
        return ModulesGroups {
            entry[1].toString(),
            entry[2].toString(),
            entry[3].toString(),
            entry[0].toInt()
        };
    });
    return target;
}

std::pair<bool, std::vector<ModulesGroups>> ModulesGroupsReader::requestModulesGroupsBrowse()
{
    DBResult result;
    std::vector<DBEntry> entries;
    std::tie(result, entries) = m_dbProcessor->requestTableData(DBTables::ModulesGroups);
    return std::make_pair(result == DBResult::OK,
                          transform(entries));
}
