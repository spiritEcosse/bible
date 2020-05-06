#include "Processor.h"
#include "Selector.h"
#include "dbmapper.h"
#include "manipulator.h"
#include <mutex>

namespace db
{

struct Processor::ProcessorPrivate
{
    Selector selector;
    Manipulator manipulator;
#ifdef BUILD_TESTS
    std::once_flag initialized;
    void insertMockData();
#endif
};

Processor::Processor()
    : m_d {new ProcessorPrivate {}}
{}

Processor::~Processor()
{}

#ifdef BUILD_TESTS
void Processor::ProcessorPrivate::insertMockData()
{
    manipulator.insertRow("ModulesGroup", {{"Language1", "Type1", "Region1"}});
    manipulator.insertRow("ModulesGroup", {{"Language2", "Type2", "Region2"}});
    manipulator.insertRow("ModulesGroup", {{"Language3", "Type3", "Region3"}});
    manipulator.insertRow("ModulesGroup", {{"Language4", "Type4", "Region4"}});
    manipulator.insertRow("ModulesGroup", {{"Language5", "Type5", "Region5"}});
    manipulator.insertRow("ModulesGroup", {{"Language6", "Type6", "Region6"}});
    manipulator.insertRow("ModulesGroup", {{"Language7", "Type7", "Region7"}});
    manipulator.insertRow("ModulesGroup", {{"Language8", "Type8", "Region8"}});
    manipulator.insertRow("ModulesGroup", {{"Language9", "Type9", "Region9"}});
}
#endif

std::pair<DBResult, std::vector<DBEntry> > Processor::requestTableData(DBTables table)
{
#ifdef BUILD_TESTS
    auto inserter = [this] {
        m_d->insertMockData();
    };
    std::call_once(m_d->initialized, inserter);
#endif
    const auto &result {m_d->selector.selectAll(TableMapper.at(table))};
    return result;
}

}
