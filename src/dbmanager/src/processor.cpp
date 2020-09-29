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
        : m_selector {new Selector {nameDb}} {}
    ProcessorPrivate() {}
    std::unique_ptr<Selector> m_selector;
    Manipulator<T> manipulator;
    std::once_flag initialized;
    void insertMockData();
};

template<class T>
Processor<T>::Processor()
    : m_d {new ProcessorPrivate {}}
{
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

//template<class T>
//void Processor<T>::ProcessorPrivate::insertMockData()
//{
////    manipulator.insertRow("ModulesGroup", {{"Language1", "Type1", "Region1"}});
////    manipulator.insertRow("ModulesGroup", {{"Language2", "Type2", "Region2"}});
////    manipulator.insertRow("ModulesGroup", {{"Language3", "Type3", "Region3"}});
////    manipulator.insertRow("ModulesGroup", {{"Language4", "Type4", "Region4"}});
////    manipulator.insertRow("ModulesGroup", {{"Language5", "Type5", "Region5"}});
////    manipulator.insertRow("ModulesGroup", {{"Language6", "Type6", "Region6"}});
////    manipulator.insertRow("ModulesGroup", {{"Language7", "Type7", "Region7"}});
////    manipulator.insertRow("ModulesGroup", {{"Language8", "Type8", "Region8"}});
////    manipulator.insertRow("ModulesGroup", {{"Language9", "Type9", "Region9"}});
//}

template<class T>
std::pair<DBTypes::DBResult, std::vector<DBTypes::DBEntry>>
Processor<T>::requestTableData(DBTypes::DBTables table)
{
    std::vector<QVariantList> result;
    const DBTypes::DBResult resultState {m_d->m_selector->selectAll(tableMapper.at(table), result)};
    return std::make_pair(resultState, std::move(result));
}

template<class T>
void Processor<T>::insertBulk(const std::vector<T>& container) const
{
    qWarning() << T::tableName();
    for (auto it = container.begin(); it != container.end(); ++it) {
        qWarning() << *it << it->url();
    }
}

}

template class db::Processor<Registry>;
