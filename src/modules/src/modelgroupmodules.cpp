#include <QtQuick>
#include "modelgroupmodules.h"
#include <QDebug>

Q_DECLARE_METATYPE(std::shared_ptr<modules::ModelModule>)

namespace modules {

    using namespace sqlite_orm;

    ModelGroupModules::ModelGroupModules()
        : m_managerGroup { new ManagerGroup {} }
    {
        m_newVersionAvailable = m_managerGroup->m_managerRegistry->hasNewRegistry();
        updateObjects();
        connect(m_managerGroup.get(), &ManagerGroup::makeGroupModulesSuccess, this, &ModelGroupModules::update);
        connect(this, &ModelGroupModules::updateDone, this, &ModelGroupModules::setUpdateCompleted);
        connect(this, &ModelGroupModules::updateDone, this, &ModelGroupModules::updateObjects);
    }

    ModelGroupModules::~ModelGroupModules() {}

    void ModelGroupModules::registerMe()
    {
        qmlRegisterType<ModelGroupModules>("bible.ModelGroupModules", 1, 0, "ModelGroupModules");
    }

    bool ModelGroupModules::newVersionAvailable() const
    {
        return m_newVersionAvailable;
    }

    bool ModelGroupModules::updateCompleted() const
    {
        return m_updateCompleted;
    }

    void ModelGroupModules::update()
    {
        try {
          auto guard = m_db->storage->transaction_guard();

          m_db->removeAll();
          int chunkSize = 2000;

          auto start = MapValueIterator(m_managerGroup->m_objects.begin());
          auto end = MapValueIterator(m_managerGroup->m_objects.end());

          while (start != end) {
              auto next = std::distance(start, end) >= chunkSize
                          ? std::next(start, chunkSize)
                          : end;

              m_db->save(start, next);
              start = next;
          }
          guard.commit();
          emit updateDone();
        } catch(const std::system_error& e) {
            qInfo() << e.what();
            emit error("An error occured.");
        }
    }

    void ModelGroupModules::setUpdateCompleted()
    {
        m_updateCompleted = true;
        emit changeUpdateCompleted();
    }

    void ModelGroupModules::updateObjects()
    {
        beginResetModel();
        objectsCount = 0;
        m_objects = m_db->storage->get_all_pointer<GroupModules>(
                    multi_order_by(
                        order_by(&GroupModules::m_region),
                        order_by(&GroupModules::getLanguageName),
                        order_by(&GroupModules::m_name)
                    ));
        endResetModel();
    }

    void ModelGroupModules::getAll()
    {
        updateObjects();
    }

    void ModelGroupModules::search(const QString& needle)
    {
        beginResetModel();
        objectsCount = 0;
        m_objects = m_db->storage->get_all_pointer<GroupModules>(
                    where(
                        like(&GroupModules::m_region, needle + "%") or
                        like(&GroupModules::m_name, needle + "%") or
                        like(&GroupModules::getLanguageName, needle + "%")
                    ),
                    multi_order_by(
                        order_by(&GroupModules::m_region),
                        order_by(&GroupModules::getLanguageName),
                        order_by(&GroupModules::m_name)
                    ));
        endResetModel();
    }

    void ModelGroupModules::downloadRegistry()
    {
        m_newVersionAvailable = false;
        emit changeNewVersionAvailable();
        QTimer::singleShot(0, m_managerGroup.get(), &ManagerGroup::downloadRegistry);
    }

    QVariant ModelGroupModules
    ::data(const QModelIndex & index, int role) const {
        QVariant data {};
        qRegisterMetaType<std::shared_ptr<ModelModule>>("std::shared_ptr<ModelModule>");

        if (!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &groupModules = m_objects.at(index.row());

        switch(role) {
            case TitleRole :
                data = std::move(groupModules->titleGroup());
                break;
            case RegionRole :
                data = std::move(groupModules->region());
                break;
            case ModulesRole :
                groupModules->m_modules.reset(new ModelModule(groupModules->m_groupId));
                data = qVariantFromValue(groupModules->m_modules.get());
                break;
            case CountModulesRole :
                data = std::move(groupModules->m_countModules);
                break;
        }

        return data;
    }

    QHash<int, QByteArray>
    ModelGroupModules::roleNames() const {
        return {
            { TitleRole, "titleGroup" },
            { RegionRole, "region" },
            { ModulesRole, "modules" },
            { CountModulesRole, "count_modules" },
        };
    }

}
