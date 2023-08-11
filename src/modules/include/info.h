//
// Created by ihor on 20.11.2022.
//

#ifndef INFO_H
#define INFO_H

#include <memory>

namespace modules {

    class Info {
    public:
        explicit Info(const QString &name, const QString &value);

        QString m_name;
        QString m_value;

        bool operator==(const Info &other) const;

#ifndef QT_NO_DEBUG_STREAM
        friend QDebug operator<<(QDebug debug, const Info &info);
#endif
    };

    using InfoShared = decltype(std::shared_ptr<Info>());
    using InfoUnique = decltype(std::unique_ptr<Info>());

}  // namespace modules
#endif  // INFO_H
