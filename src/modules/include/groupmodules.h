#ifndef GROUPMODULES_H
#define GROUPMODULES_H

#include <memory>

#include "locallanguage.h"
#include "module.h"

namespace modules {

    namespace tests
    {
        class tst_GroupModules;
    }


    class GroupModules
    {
    public:
        GroupModules();
        GroupModules(const QJsonObject& qJsonModule);
        GroupModules(const QString& language, const QString& name, const QString& region = "");
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
        uint modulesCount();
        std::string nameToStdString();
        static QString parseName(const QString& name);
        void addModule(const Module& modules);

        bool operator==(const GroupModules& other) const;

        core::LocalLanguage m_language;
        QString m_name;
        QString m_region;
        QString m_languageName;
        int m_id;

    #ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const GroupModules& groupModules);
    #endif

    private:
        friend class tests::tst_GroupModules;

        void cleanName();
        std::vector<Module> m_modules;
    };

}

#endif // GROUPMODULES_H
