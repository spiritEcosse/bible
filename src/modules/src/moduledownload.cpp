#include "moduledownload.h"
#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif

namespace modules {

    ModuleDownload::ModuleDownload(
            const QString& abbreviation,
            bool selecting,
            bool downloaded,
            int moduleId,
            int groupId,
            int id)
        : m_abbreviation { std::move(abbreviation) },
          m_moduleId { moduleId },
          m_selecting { selecting },
          m_downloaded { downloaded },
          m_groupId { groupId },
          m_id { id }
    {}

    bool ModuleDownload::operator==(const ModuleDownload &other) const
    {
        return m_moduleId == other.m_moduleId &&
                m_selecting == other.m_selecting &&
                m_downloaded == other.m_downloaded &&
                m_groupId == other.m_groupId &&
                m_id == other.m_id;
    }

    #ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug debug, const ModuleDownload& data)
    {
        return debug << data.m_abbreviation << data.m_moduleId << data.m_selecting
                     << data.m_downloaded << data.m_groupId << data.m_id;
    }
    #endif

}
