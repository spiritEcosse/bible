#include <QJsonObject>
#include <QRegularExpression>
#include <qmath.h>

#include "module.h"

#define MODULES_DATE_FORMAT QString("yyyy-MM-dd")


Module::Module(const QJsonObject& moduleJson)
    : m_name { moduleJson.value("fil").toString() },
      m_description { moduleJson.value("des").toString() },
      m_abbreviation { moduleJson.value("abr").toString() },
      m_information { moduleJson.value("inf").toString() },
      m_comment { moduleJson.value("cmt").toString() },
      m_copyright { moduleJson.value("lic").toString() },
      m_hidden { moduleJson.value("hid").toBool() },
      m_defaultDownload { moduleJson.value("def").toBool() },
      m_languageShow { moduleJson.value("aln").toString() }
{
    convertSize(moduleJson.value("siz").toString());
    convertUpdate(moduleJson.value("upd").toString());
}

double Module::size() const
{
    return m_size;
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
