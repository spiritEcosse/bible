//
// Created by ihor on 21.11.2022.
//

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif

#include "info.h"

namespace modules {

    Info::Info(const QString &name, const QString &value) {}

    bool Info::operator==(const Info &other) const {
        return m_name == other.m_name && m_value == other.m_value;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug debug, const Info &info) {
        return debug << info.m_name << info.m_value;
    }
#endif

}  // namespace modules