#pragma once
#include <memory>
#include "dbtypes.h"

class QString;

namespace db {

class ConnectionManager
{
public:
    ConnectionManager();
    ConnectionManager(const QString& namedb);

    ~ConnectionManager();
    bool isValid() const;

private:
    struct ConnectionManagerPrivate;
    std::unique_ptr<ConnectionManagerPrivate> m_d;
};

}
