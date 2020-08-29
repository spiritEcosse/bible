#pragma once
#include <dbtypes.h>
#include <memory>

namespace db
{
class Processor
{
public:
    Processor();
    Processor(const QString& nameDb);
    ~Processor();
    std::pair<DBTypes::DBResult,
              std::vector<DBTypes::DBEntry>> requestTableData(DBTypes::DBTables table);

private:
    struct ProcessorPrivate;
    std::unique_ptr<ProcessorPrivate> m_d;
};
}
