#include <QSqlDatabase>

#ifndef USERDB_H
#define USERDB_H


class UserDB
{
public:
    static UserDB* getInstance();
    QSqlDatabase db;
private:
    UserDB();
    UserDB(const UserDB&);
    UserDB& operator=(const UserDB&);
};

#endif // USERDB_H
