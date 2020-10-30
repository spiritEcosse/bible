#include <QJsonObject>
#include <QRegularExpression>
#include <qmath.h>

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif

#include "module.h"

#define MODULES_DATE_FORMAT QString("yyyy-MM-dd")


namespace modules {

    Module::Module(const QJsonObject& moduleJson)
        : m_name { moduleJson.value("fil").toString() },
          m_description { moduleJson.value("des").toString() },
          m_abbreviation { moduleJson.value("abr").toString() },
          m_languageShow { moduleJson.value("aln").toString() },
          m_information { moduleJson.value("inf").toString() },
          m_comment { moduleJson.value("cmt").toString() },
          m_copyright { moduleJson.value("lic").toString() },
          m_hidden { moduleJson.value("hid").toBool() },
          m_defaultDownload { moduleJson.value("def").toBool() }
    {
        if (m_name.isEmpty() || m_description.isEmpty() || m_abbreviation.isEmpty()) {
            throw(ModuleInvalidData("Invalid data."));
        }

        convertSize(moduleJson.value("siz").toString());
        convertUpdate(moduleJson.value("upd").toString());
    }

    Module::Module(
            const QString& name,
            const QString& description,
            const QString& abbreviation,
            const int& idGroupModules,
            const double& size,
            const QString& languageShow,
            const QString& information,
            const QString& comment,
            const QString& copyright,
            const QDate& update,
            const bool& hidden,
            const bool& defaultDownload)
        : m_name { std::move(name) },
          m_description { std::move(description) },
          m_abbreviation { std::move(abbreviation) },
          m_idGroupModules { std::move(idGroupModules) },
          m_size { std::move(size) },
          m_languageShow { std::move(languageShow) },
          m_information { std::move(information) },
          m_comment { std::move(comment) },
          m_copyright { std::move(copyright) },
          m_update { std::move(update) },
          m_hidden { std::move(hidden) },
          m_defaultDownload { std::move(defaultDownload) }
    {}

    bool Module::operator==(const Module &other) const
    {
        return m_name == other.m_name &&
                m_description == other.m_description &&
                m_abbreviation == other.m_abbreviation &&
                m_size == other.m_size &&
                m_information == other.m_information &&
                m_comment == other.m_comment &&
                m_copyright == other.m_copyright &&
                m_update == other.m_update &&
                m_hidden == other.m_hidden &&
                m_defaultDownload == other.m_defaultDownload &&
                m_languageShow == other.m_languageShow &&
                m_idGroupModules == other.m_idGroupModules;
    }

    void Module::convertSize(const QString& str)
    {
        QRegularExpression re("^([+-]?\\d*\\.?\\d+)(\\w{1})*$", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = re.match(str);
        QStringList dimensions = {"K", "M", "G"};

        if (match.hasMatch()) {
            m_size = match.captured(1).toDouble();
            QString dimension;
            match.captured(2).toUpper().swap(dimension);
            m_size *= qPow(1024, dimensions.indexOf(dimension) + 1);
        }
    //WARNING: replace on formattedDataSize. This function was introduced in Qt 5.10.
    }

    void Module::convertUpdate(const QString& update)
    {
        m_update = QDate::fromString(update, MODULES_DATE_FORMAT);
    }

    QString Module::nativeLanguageNameShow() const
    {
        return m_languageShow.nativeLanguageName();
    }

    QString Module::languageNameShow() const
    {
        return m_languageShow.languageInString();
    }

    #ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug debug, const Module& module)
    {
        return debug << module.m_name << module.m_description << module.m_abbreviation << module.m_size <<
                        module.m_information << module.m_comment << module.m_copyright << module.m_update <<
                        module.m_hidden << module.m_defaultDownload << module.m_languageShow;
    }
    #endif

}
