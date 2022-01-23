#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QDate>
#include <memory>
#include "locallanguage.h"
#include "booksmodel.h"

class QJsonObject;

namespace modules {

    namespace tests {
        class tst_Module;
    }

    class Module
    {
    public:
        Module() = default;
        Module(
                const QString& name,
                const QString& description = "",
                const QString& abbreviation = "",
                int idGroupModules = 0,
                double size = 0,
                const QString& languageShow = "",
                const QString& information = "",
                const QString& comment = "",
                const QString& copyright = "",
                const QDate& update = QDate(),
                bool hidden = false,
                bool defaultDownload = false,
                bool downloaded = false,
                bool selected = false,
                bool active = false,
                int id = 0);
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
        QString m_count;
        QString m_copyright;
        QDate m_update;
        bool m_hidden;
        bool m_defaultDownload;
        bool m_downloaded = false;
        bool m_selected = false;
        bool m_active = false;
        int m_id = 0;
        std::shared_ptr<BooksModel> m_books;

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
