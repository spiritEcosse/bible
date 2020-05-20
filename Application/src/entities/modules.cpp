#include <QJsonObject>
#include <QRegularExpression>
#include <qmath.h>

#include "modules.h"

Modules::Modules() {}

Modules::Modules(const QJsonObject& qJsonModule)
    : m_name { qJsonModule.value("fil").toString() },
      m_description { qJsonModule.value("des").toString() },
      m_language { new LocalLanguage { qJsonModule.value("lng").toString() } }
{
    convertSize(qJsonModule.value("siz").toString());
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
