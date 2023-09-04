#include "modelgroupmodules.h"
#include <QDebug>
#include <QtQuick>
#include <memory>

Q_DECLARE_METATYPE(std::shared_ptr<modules::ModelModule>)

namespace modules {

    static std::once_flag flagInitGroupModules;
    static std::once_flag flagPostEmptyModel;

    using namespace sqlite_orm;

    ModelGroupModules::ModelGroupModules() :
        m_managerGroup{new ManagerGroup{}}, m_managerRegistry{new ManagerRegistry{}}, m_modelModule{new ModelModule{}} {
        connect(m_managerRegistry.get(),
                &ManagerRegistry::retrieveDataSuccess,
                m_modelModule.get(),
                &ModelModule::getExtraFieldsFromDb);
        connect(m_managerRegistry.get(),
                &ManagerRegistry::retrieveDataSuccess,
                m_managerGroup.get(),
                &ManagerGroup::makeCollections);
        connect(m_managerGroup.get(), &ManagerGroup::makeGroupModulesSuccess, this, &ModelGroupModules::update);
        connect(m_managerGroup.get(), &ManagerGroup::makeModulesSuccess, m_modelModule.get(), &ModelModule::update);
        connect(this, &ModelGroupModules::updateDone, this, &ModelGroupModules::setUpdateCompleted);
        connect(this, &ModelGroupModules::updateDone, m_managerGroup.get(), &ManagerGroup::clearObjects);
        connect(this, &ModelGroupModules::updateDone, this, &ModelGroupModules::updateObjects);
    }

    void ModelGroupModules::init() {
        std::call_once(flagInitGroupModules, [this]() {
            updateObjects();
        });
    }

    ModelGroupModules::~ModelGroupModules() {}

    void ModelGroupModules::registerMe() {
        qmlRegisterType<ModelGroupModules>("bible.ModelGroupModules", 1, 0, "ModelGroupModules");
    }

    QString ModelGroupModules::needle() const {
        return m_needle;
    }

    void ModelGroupModules::update() {
        try {
            auto guard = m_db->storage->transaction_guard();

            m_db->removeAll();
            int chunkSize = 2000;
            qDebug() << "ModelGroupModules: update";

            auto start = MapValueIterator(m_managerGroup->m_objects.begin());
            auto end = MapValueIterator(m_managerGroup->m_objects.end());

            while(start != end) {
                auto next = std::distance(start, end) >= chunkSize ? std::next(start, chunkSize) : end;

                m_db->save(start, next);
                start = next;
            }
            guard.commit();
            emit updateDone();
        } catch(const std::system_error &e) {
            qInfo() << e.what();
            emit error("An error occurred.");
        } catch(...) {
            throw;
        }
    }

    void ModelGroupModules::doSearchByModules() {
        beginResetModel();
        objectsCount = 0;

        m_objects = m_db->storage->get_all_pointer<GroupModules>(
            inner_join<Module>(on(c(&Module::m_idGroupModules) == &GroupModules::m_groupId)),
            where(like(&Module::m_abbreviation, m_needle + "%")),
            group_by(&GroupModules::m_groupId),
            multi_order_by(order_by(&GroupModules::m_region),
                           order_by(&GroupModules::getLanguageName),
                           order_by(&GroupModules::m_name)));
        endResetModel();
    }

    void ModelGroupModules::doSearchByGroups() {
        beginResetModel();
        objectsCount = 0;

        m_objects = m_db->storage->get_all_pointer<GroupModules>(
            where(like(&GroupModules::m_region, m_needle + "%") or like(&GroupModules::m_name, m_needle + "%") or
                  like(&GroupModules::getLanguageName, m_needle + "%")),
            multi_order_by(order_by(&GroupModules::m_region),
                           order_by(&GroupModules::getLanguageName),
                           order_by(&GroupModules::m_name)));
        endResetModel();
    }

    void ModelGroupModules::search(const QString &needle) {
        setFieldSearch(needle);

        if(m_needle.length() >= 2) {
            emit changeNeedle();

            switch(m_entitySearch) {
                case ModuleSearch:
                    doSearchByModules();
                    qDebug() << "ModuleSearch";
                    break;
                case GroupSearch:
                    doSearchByGroups();
                    qDebug() << "doSearchByGroups";
                    break;
            }
        }
    }

    // m_updateCompleted
    void ModelGroupModules::setUpdateCompleted() {
        m_updateCompleted = true;
        qDebug() << "setUpdateCompleted";
        emit changeUpdateCompleted();
    }

    bool ModelGroupModules::updateCompleted() const {
        return m_updateCompleted;
    }

    //m_postEmptyModelCompleted
    void ModelGroupModules::setPostEmptyModelCompleted() {
        m_postEmptyModelCompleted = true;
        emit changePostEmptyModelCompleted();
    }

    bool ModelGroupModules::postEmptyModelCompleted() const {
        return m_postEmptyModelCompleted;
    }

    void ModelGroupModules::updateObjects() {
        beginResetModel();
        objectsCount = 0;
        m_objects =
            m_db->storage->get_all_pointer<GroupModules>(multi_order_by(order_by(&GroupModules::m_region),
                                                                        order_by(&GroupModules::getLanguageName),
                                                                        order_by(&GroupModules::m_name)));
        endResetModel();
    }

    void ModelGroupModules::getAll() {
        m_entitySearch = EntitySearch::GroupSearch;
        updateObjects();
    }

    void ModelGroupModules::postEmptyModel() {
        std::call_once(flagPostEmptyModel, [this]() {
            if(m_db->count() == 0) {
                connect(this,
                        &ModelGroupModules::changeUpdateCompleted,
                        &ModelGroupModules::setPostEmptyModelCompleted);
                downloadRegistry();
            }
        });
    }

    void ModelGroupModules::downloadRegistry() {
        //        m_updateCompleted = false;
        //        emit changeUpdateCompleted();

        QTimer::singleShot(0, m_managerRegistry.get(), &ManagerRegistry::download);
    }

    bool ModelGroupModules::searchByModules() const {
        return m_entitySearch == ModuleSearch;
    }

    bool ModelGroupModules::searchByGroups() const {
        return m_entitySearch == GroupSearch;
    }

    QString ModelGroupModules::getNameJson() {
        return QString("downloads");
    }

    QVariant ModelGroupModules ::data(const QModelIndex &index, int role) const {
        QVariant data{};
        qRegisterMetaType<std::shared_ptr<ModelModule>>("std::shared_ptr<ModelModule>");

        if(!index.isValid() || index.row() > rowCount(index)) {
            return data;
        }

        const auto &groupModules = m_objects.at(index.row());

        switch(role) {
            case TitleRole:
                data = groupModules->titleGroup();
                break;
            case RegionRole:
                data = groupModules->region();
                break;
            case Hidden:
                data = groupModules->m_name != "Translations";
                break;
            case ModulesRole:
                if(groupModules->m_modules == nullptr) {
                    groupModules->m_modules =
                        std::make_shared<ModelModule>(groupModules->m_groupId, searchByModules() ? m_needle : "");
                }
#ifdef Qt6_FOUND
                data = QVariant::fromValue(groupModules->m_modules.get());
#else
                data = qVariantFromValue(groupModules->m_modules.get());
#endif
                break;
            case CountModulesRole:
                data = groupModules->m_countModules;
                break;
            case IdRole:
                data = groupModules->m_id;
                break;
            case GroupIdRole:
                data = groupModules->m_groupId;
                break;
            default:
                data = QVariant();
        }

        return data;
    }

    QHash<int, QByteArray> ModelGroupModules::roleNames() const {
        return {
            {TitleRole, "titleGroup"},
            {RegionRole, "region"},
            {ModulesRole, "modules"},
            {CountModulesRole, "count_modules"},
            {IdRole, "id"},
            {GroupIdRole, "group_id"},
            {Hidden, "groupHidden"},
        };
    }

    void ModelGroupModules::setFieldSearch(const QString &needle) {
        QRegularExpression re("^([m|M]\\s)([\\w]*)$", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = re.match(std::move(needle));

        if(match.hasMatch()) {
            m_entitySearch = EntitySearch::ModuleSearch;
            m_needle = match.captured(2);
        } else {
            m_entitySearch = EntitySearch::GroupSearch;
            m_needle = std::move(needle);
        }
    }

}  // namespace modules
