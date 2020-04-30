#pragma once
#include <dbtypes.h>
#include <memory>

namespace db
{
class Processor
{
public:
    Processor();
    ~Processor();
    std::pair <DBResult, std::vector<DBEntry>> requestTableData(DBTables table);

private:
    struct ProcessorPrivate;
    std::unique_ptr<ProcessorPrivate> m_d;
};
}
