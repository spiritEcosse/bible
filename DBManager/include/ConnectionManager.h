#pragma once
#include <memory>

namespace db {

class ConnectionManager
{
public:
    ConnectionManager();

    ~ConnectionManager();
    bool isValid() const;

private:
    struct ConnectionManagerPrivate;
    std::unique_ptr<ConnectionManagerPrivate> m_d;
};

}
