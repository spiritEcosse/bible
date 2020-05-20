#include <QJsonObject>
#include <QRegularExpression>
#include <qmath.h>
#include <QDebug>

#include "global.h"
#include "modules.h"

Modules::Modules() {}

Modules::Modules(const QJsonObject& qJsonModule)
    : m_name { qJsonModule.value("fil").toString() },
      m_description { qJsonModule.value("des").toString() },
      m_abbreviation { qJsonModule.value("abr").toString() },
      m_information { qJsonModule.value("inf").toString() },
      m_languageShow { new LocalLanguage { qJsonModule.value("aln").toString() } }
{
    convertSize(qJsonModule.value("siz").toString());
    convertUpdate(qJsonModule.value("upd").toString());
}

double Modules::size() const
{
    return m_size;
}

void Modules::convertSize(const QString& str)
{
    QRegularExpression re("^([+-]?\\d*\\.?\\d+)(\\w{1})*$", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(str);
    QStringList dimensions = {"K", "M", "G"};

    if (match.hasMatch()) {
        m_size = match.captured(1).toDouble();
        QString dimension = match.captured(2).toUpper();
        m_size *= qPow(1024, dimensions.indexOf(dimension) + 1);
    }
//WARNING: replace on formattedDataSize. This function was introduced in Qt 5.10.
}

void Modules::convertUpdate(const QString& update)
{
    m_update = QDate::fromString(update, MODULES_DATE_FORMAT);
}

QString Modules::nativeLanguageNameShow() const
{
    return m_languageShow->nativeLanguageName();
}

QString Modules::languageNameShow() const
{
    return m_languageShow->languageToString(m_languageShow->language());
}
