#ifndef GROUPMODULES_H
#define GROUPMODULES_H

#include <memory>
#include <unordered_map>
#include <map>
#include "locallanguage.h"

namespace modules {

    namespace tests
    {
        class tst_GroupModules;
    }

    class ModelModule;

    class GroupModules
    {
    public:
        GroupModules();
        GroupModules(const QJsonObject& qJsonModule);
        GroupModules(const QString& language, const QString& name, const QString& region = "", int groupId = 0);
        ~GroupModules();
        QString nativeLanguageName() const;
        QString languageName() const;
        QLocale language() const;
        QString region() const;
        std::string regionToStdString() const;
        QString languageCode() const;
        inline void setLanguageName(const QString&) {}
        QString getLanguageName() const;
        QString titleGroup() const;
        std::string languageCodeToStdString() const;
        QString name() const;
        std::string nameToStdString();
        static QString parseName(const QString& name);

        bool operator==(const GroupModules& other) const;

        core::LocalLanguage m_language;
        QString m_name;
        QString m_region;
        QString m_languageName;
        int m_id;
        int m_groupId;
        int m_countModules = 0;

    #ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const GroupModules& groupModules);
    #endif

        std::shared_ptr<ModelModule> m_modules;
    private:
        friend class tests::tst_GroupModules;

        void cleanName();
    };

    using GroupModulesShared = decltype(std::shared_ptr<GroupModules>());
    using GroupModulesUnique = decltype(std::unique_ptr<GroupModules>());

    // WARNING: replace this three on one class
    struct MGKey {
        std::string name;
        std::string language;
        std::string region;
    };

    struct MGKeyHash {
        std::size_t operator()(const MGKey& mgKey) const
        {
            using namespace std;
            return ((hash<string>()(mgKey.name)
                     ^ (hash<string>()(mgKey.language) << 1)) >> 1)
                     ^ (hash<string>()(mgKey.region) << 1);
        }
    };

    struct MGKeyEqual {
        bool operator()(const MGKey& lhs, const MGKey& rhs) const
        {
            return lhs.name == rhs.name && lhs.language == rhs.language && lhs.region == rhs.region;
        }
    };

    using GroupModulesMap = decltype(std::unordered_map<MGKey, GroupModulesUnique, MGKeyHash, MGKeyEqual>());
    using MapIterator = decltype(std::unordered_map<MGKey, GroupModulesUnique, MGKeyHash, MGKeyEqual>::iterator());

    class MapValueIterator : public MapIterator
    {
    public:
        using value_type = GroupModules;
        using pointer = value_type *;
        using reference = value_type &;

        MapValueIterator ( const MapIterator &it_ ) : MapIterator ( it_ ) { };

        pointer operator -> ( ) { return ( GroupModules * const ) &(*(MapIterator::operator -> ( )->second)); }
        reference operator * ( ) { return *(MapIterator::operator * ( ).second); }
    };

}

#endif // GROUPMODULES_H
