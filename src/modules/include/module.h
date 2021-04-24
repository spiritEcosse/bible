#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QDate>
#include <memory>

#include "locallanguage.h"

class QJsonObject;

namespace modules {

    namespace tests {
        class tst_Module;
    }

    class ModuleInvalidData: virtual public std::exception {

    protected:

        std::string error_message;      ///< Error message

    public:

        /** Constructor (C++ STL string, int, int).
         *  @param msg The error message
         *  @param err_num Error number
         *  @param err_off Error offset
         */
        explicit
        ModuleInvalidData(const std::string& msg)
            : error_message(msg)
            {}

        /** Destructor.
         *  Virtual to allow for subclassing.
         */
        virtual ~ModuleInvalidData() throw () {}

        /** Returns a pointer to the (constant) error description.
         *  @return A pointer to a const char*. The underlying memory
         *  is in possession of the Except object. Callers must
         *  not attempt to free the memory.
         */
        virtual const char* what() const throw () {
           return error_message.c_str();
        }

    };

    class Module
    {
    public:
        Module() = default;
        Module(
                const QString& name,
                const QString& description = "",
                const QString& abbreviation = "",
                const int& idGroupModules = 0,
                const double& size = 0,
                const QString& languageShow = "",
                const QString& information = "",
                const QString& comment = "",
                const QString& copyright = "",
                const QDate& update = QDate(),
                const bool& hidden = false,
                const bool& defaultDownload = false,
                const int& id = 0);
        Module(const QJsonObject& qJsonModule);
        QString nativeLanguageNameShow() const;
        QString languageNameShow() const;
        QString getLanguageShowName() const;
        void setLanguageShowName(const QString&);

        QString m_name;
        QString m_description;
        QString m_abbreviation;
        int m_idGroupModules = 0;
        double m_size = 0;
        core::LocalLanguage m_languageShow;
        QString m_information;
        QString m_comment;
        QString m_copyright;
        QDate m_update;
        bool m_hidden;
        bool m_defaultDownload;
        int m_id = 0;

        bool operator==(const Module& other) const;

        void convertSize(const QString& str);
        void convertUpdate(const QString& update);

    #ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Module& module);
    #endif
    };

    using ModuleShared = decltype(std::shared_ptr<Module>());
    using ModuleUnique = decltype(std::unique_ptr<Module>());

}

#endif // MODULE_H
