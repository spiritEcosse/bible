#include <QtQuick>
#include "modelgroupmodules.h"
#include <QDebug>

Q_DECLARE_METATYPE(std::shared_ptr<modules::ModelModule>)

namespace modules {

    static std::once_flag flagInitGroupModules;

    using namespace sqlite_orm;

    ModelGroupModules::ModelGroupModules()
        : m_managerGroup { new ManagerGroup {} },
          m_managerRegistry { new ManagerRegistry {} },
          m_modelModule { new ModelModule {} }
    {
        m_newVersionAvailable = m_managerRegistry->hasNewRegistry();
        connect(m_managerRegistry.get(), &ManagerRegistry::retrieveDataSuccess, m_managerGroup.get(), &ManagerGroup::makeCollections);
        connect(m_managerRegistry.get(), &ManagerRegistry::retrieveDataSuccess, m_modelModule.get(), &ModelModule::getExtraFieldsFromDb);
        connect(m_managerGroup.get(), &ManagerGroup::makeGroupModulesSuccess, this, &ModelGroupModules::update);
        connect(m_managerGroup.get(), &ManagerGroup::makeModulesSuccess, m_modelModule.get(), &ModelModule::update);
        connect(this, &ModelGroupModules::updateDone, this, &ModelGroupModules::setUpdateCompleted);
        connect(this, &ModelGroupModules::updateDone, this, &ModelGroupModules::updateObjects);
    }

    void ModelGroupModules::init()
    {
        std::call_once(flagInitGroupModules, [this]() {
            updateObjects();
        });
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

    QString ModelGroupModules::needle() const
    {
        return m_needle;
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

    void ModelGroupModules::doSearchByModules()
    {
        beginResetModel();
        objectsCount = 0;

        m_objects = m_db->storage->get_all_pointer<GroupModules>(
                inner_join<Module>(on(c(&Module::m_idGroupModules) == &GroupModules::m_groupId)),
                where(
                    like(&Module::m_abbreviation, m_needle + "%")
                ),
                group_by(&GroupModules::m_groupId),
                multi_order_by(
                    order_by(&GroupModules::m_region),
                    order_by(&GroupModules::getLanguageName),
                    order_by(&GroupModules::m_name)
                ));
        endResetModel();
    }

    void ModelGroupModules::doSearchByGroups()
    {
        beginResetModel();
        objectsCount = 0;

        m_objects = m_db->storage->get_all_pointer<GroupModules>(
            where(
                like(&GroupModules::m_region, m_needle + "%") or
                like(&GroupModules::m_name, m_needle + "%") or
                like(&GroupModules::getLanguageName, m_needle + "%")
            ),
            multi_order_by(
                order_by(&GroupModules::m_region),
                order_by(&GroupModules::getLanguageName),
                order_by(&GroupModules::m_name)
            ));
        endResetModel();
    }

    void ModelGroupModules::search(const QString& needle)
    {
        setFieldSearch(needle);

        if (m_needle.length() >= 2) {
            emit changeNeedle();

            switch (m_entitySearch) {
                case ModuleSearch :
                    doSearchByModules();
                    break;
                case GroupSearch :
                    doSearchByGroups();
                    break;
            }
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
        m_entitySearch = EntitySearch::GroupSearch;
        updateObjects();
    }

    void ModelGroupModules::downloadRegistry()
    {
        m_newVersionAvailable = false;
        emit changeNewVersionAvailable();
        QTimer::singleShot(0, m_managerRegistry.get(), &ManagerRegistry::download);
    }

    bool ModelGroupModules::searchByModules() const
    {
        return m_entitySearch == ModuleSearch;
    }

    bool ModelGroupModules::searchByGroups() const
    {
        return m_entitySearch == GroupSearch;
    }

    const QString ModelGroupModules::getNameJson() { return QString("downloads"); }

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
                if (groupModules->m_modules == nullptr) {
                    groupModules->m_modules.reset(
                                new ModelModule(groupModules->m_groupId, searchByModules() ? m_needle : "")
                                );
                }
                data = qVariantFromValue(groupModules->m_modules.get());
                break;
            case CountModulesRole :
                data = std::move(groupModules->m_countModules);
                break;
            case IdRole :
                data = std::move(groupModules->m_id);
                break;
            case GroupIdRole :
                data = std::move(groupModules->m_groupId);
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
            { IdRole, "id" },
            { GroupIdRole, "group_id" },
        };
    }

    void ModelGroupModules::setFieldSearch(const QString& needle)
    {
        QRegularExpression re("^([m|M]\\s)([\\w]*)$", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = re.match(std::move(needle));

        if (match.hasMatch())
        {
            m_entitySearch = EntitySearch::ModuleSearch;
            m_needle = match.captured(2);
        } else {
            m_entitySearch = EntitySearch::GroupSearch;
            m_needle = std::move(needle);
        }
    }

}
