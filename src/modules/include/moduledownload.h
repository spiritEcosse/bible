#ifndef MODULEDOWNLOAD_H
#define MODULEDOWNLOAD_H

#include <memory>
#include <QString>
#include <QObject>

namespace modules {

    class ModuleDownload : public QObject
    {
        Q_OBJECT
    public:
        ModuleDownload() = default;
        ModuleDownload(
                const QString& abbreviation,
                bool selecting = false,
                bool downloaded = false,
                int moduleId = 0,
                int groupId = 0,
                int id = 0
                );

        QString m_abbreviation;
        int m_moduleId;
        bool m_selecting;
        bool m_downloaded;
        int m_groupId;
        int m_id;

        bool operator==(const ModuleDownload& other) const;
    #ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const ModuleDownload& moduleDownload);
    #endif
    };

    using ModuleDownloadShared = decltype(std::shared_ptr<ModuleDownload>());
    using ModuleDownloadUnique = decltype(std::unique_ptr<ModuleDownload>());
    using ModuleDownloadUniqueIter = decltype(std::vector<ModuleDownloadUnique>::iterator());

}

#endif // MODULEDOWNLOAD_H
