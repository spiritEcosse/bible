#include "Processor.h"
#include "Selector.h"
#include "dbmapper.h"

namespace db
{

struct Processor::ProcessorPrivate
{
    Selector selector;
};

Processor::Processor()
    : m_d {new ProcessorPrivate {}}
{}

Processor::~Processor()
{}

std::pair<DBResult, std::vector<DBEntry> > Processor::requestTableData(DBTables table)
{
    const auto &result {m_d->selector.selectAll(TableMapper.at(table))};
    return result;
}

}
