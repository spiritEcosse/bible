#include "Selector.h"
#include "dbmapper.h"
#include "manipulator.h"
#include <mutex>
#include <QDebug>

#include "registry.h"
#include "processor.h"

namespace db
{

template<class T>
struct Processor<T>::ProcessorPrivate
{
    ProcessorPrivate(const QString& nameDb)
        : m_selector {new Selector<T> {nameDb}} {}
    ProcessorPrivate() {}
    std::unique_ptr<Selector<T>> m_selector;
    Manipulator<T> manipulator;
    void save(std::vector<T>& container);
    std::once_flag initialized;
    void insertMockData();
};

template<class T>
Processor<T>::Processor()
    : m_d {new ProcessorPrivate {}}
{
//    for (int i = 0; i < 1000; i++) {
//        m_d->manipulator.insertRow("modules_groups", {{"Language1", "Type1", "Region1"}});
//    }

//    auto inserter = [this] {
//        m_d->insertMockData();
//    };
//    std::call_once(m_d->initialized, inserter);
}

template<class T>
Processor<T>::~Processor() {}

template<class T>
Processor<T>::Processor(const QString& nameDb)
    : m_d {new ProcessorPrivate {nameDb}}
{
//    auto inserter = [this] {
//        m_d->insertMockData();
//    };
//    std::call_once(m_d->initialized, inserter);
}

template<class T>
void Processor<T>::ProcessorPrivate::insertMockData()
{
//    for (int i = 0; i < 1000; i++) {
//        qWarning() << "ProcessorPrivate";
//        manipulator.insertRow("ModulesGroup", {{"Language1", "Type1", "Region1"}});
//    }
}

template<class T>
std::pair<DBTypes::DBResult, std::vector<DBTypes::DBEntry>>
Processor<T>::requestTableData(DBTypes::DBTables table)
{
    std::vector<QVariantList> result;
    const DBTypes::DBResult resultState {m_d->m_selector->selectAll(tableMapper.at(table), result)};
    return std::make_pair(resultState, std::move(result));
}

template<class T>
void Processor<T>::save(std::vector<T>& container) const
{
    m_d->save(container);
}

template<class T>
void db::Processor<T>::ProcessorPrivate::save(std::vector<T> &container)
{
    manipulator.save(container);
}

}

template class db::Processor<Registry>;
