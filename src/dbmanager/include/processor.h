#pragma once
#include <dbtypes.h>
#include <memory>
#include <QDebug>

namespace db
{

template <class T>
class Processor
{
public:
    Processor();
    Processor(const QString& nameDb);
    ~Processor();
    std::pair<DBTypes::DBResult,
              std::vector<DBTypes::DBEntry>> requestTableData(DBTypes::DBTables table);
    void save(std::vector<T>& container) const;
private:
    struct ProcessorPrivate;
    std::unique_ptr<ProcessorPrivate> m_d;
};

}
