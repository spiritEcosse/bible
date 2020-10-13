#ifndef MODULE_H
#define MODULE_H

#include <QString>
#include <QDate>

#include "locallanguage.h"

class QJsonObject;

namespace modules {

    namespace tests {
        class tst_Module;
    }

    class Module
    {
    public:
        Module() = default;
        Module(const QJsonObject& qJsonModule);
        double size() const;
        QString nativeLanguageNameShow() const;
        QString languageNameShow() const;

        QString m_name;
        QString m_description;
        QString m_abbreviation;
        double m_size = 0;
        QString m_information;
        QString m_comment;
        QString m_copyright;
        QDate m_update;
        bool m_hidden;
        bool m_defaultDownload;
        core::LocalLanguage m_languageShow;
        int m_id;
    private:
        friend class tst_Module;

        void convertSize(const QString& str);
        void convertUpdate(const QString& update);
    };

}

#endif // MODULE_H
