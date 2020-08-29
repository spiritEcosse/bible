#include "Processor.h"
#include "Selector.h"
#include "dbmapper.h"
#include "manipulator.h"
#include <mutex>

namespace db
{

struct Processor::ProcessorPrivate
{
    ProcessorPrivate(const QString& nameDb)
        : m_selector {new Selector {nameDb}} {}
    ProcessorPrivate() {}
    std::unique_ptr<Selector> m_selector;
    Manipulator manipulator;
    std::once_flag initialized;
    void insertMockData();
};

Processor::Processor()
    : m_d {new ProcessorPrivate {}}
{
//    auto inserter = [this] {
//        m_d->insertMockData();
//    };
//    std::call_once(m_d->initialized, inserter);
}

Processor::Processor(const QString& nameDb)
    : m_d {new ProcessorPrivate {nameDb}}
{
//    auto inserter = [this] {
//        m_d->insertMockData();
//    };
//    std::call_once(m_d->initialized, inserter);
}

Processor::~Processor() {}

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

std::pair<DBTypes::DBResult, std::vector<DBTypes::DBEntry>>
Processor::requestTableData(DBTypes::DBTables table)
{
    std::vector<QVariantList> result;
    const DBTypes::DBResult resultState {m_d->m_selector->selectAll(tableMapper.at(table), result)};
    return std::make_pair(resultState, std::move(result));
}

}
