#include <QJsonObject>
#include <QRegularExpression>
#include <qmath.h>
#include <QDebug>

#include "global.h"
#include "modules.h"


Modules::Modules(const QJsonObject& qJsonModule)
    : m_languageShow { qJsonModule.value("aln").toString() }
{
    qJsonModule.value("fil").toString().swap(m_name);
    qJsonModule.value("abr").toString().swap(m_abbreviation);
    qJsonModule.value("des").toString().swap(m_description);
    qJsonModule.value("inf").toString().swap(m_information);
    qJsonModule.value("cmt").toString().swap(m_comment);
    qJsonModule.value("lic").toString().swap(m_copyright);
    m_hidden = qJsonModule.value("hid").toBool();
    m_defaultDownload = qJsonModule.value("def").toBool();

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
        QString dimension;
        match.captured(2).toUpper().swap(dimension);
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
    return m_languageShow.nativeLanguageName();
}

QString Modules::languageNameShow() const
{
    return m_languageShow.languageInString();
}
