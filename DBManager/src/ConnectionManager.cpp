#include "ConnectionManager.h"
#include "dbtypes.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

#include <QDebug>

#include <QStandardPaths>
#include <QDir>


namespace db {

namespace
{
    class DBCloser {
    public:
        void operator()(QSqlDatabase *db)
        {
            db->close();
            QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
            delete db;
        }
    };
}

struct ConnectionManager::ConnectionManagerPrivate
{
    std::unique_ptr<QSqlDatabase, DBCloser> database;
    QString dbPath;
    bool isValid {true};
    DBState state {DBState::OK};
    bool setup();
    bool setupWorkspace();
    bool setupTables();
};

ConnectionManager::ConnectionManager()
    : m_d(new ConnectionManagerPrivate {})
{
    m_d->isValid = m_d->setup();
}

ConnectionManager::~ConnectionManager()
{

}

bool ConnectionManager::isValid() const
{
    return m_d->isValid;

}

bool ConnectionManager::ConnectionManagerPrivate::setup() // WARNING : using chain : https://en.wikipedia.org/wiki/Variadic_template
{
    static const QString driver {"QSQLITE"};

    if (!QSqlDatabase::isDriverAvailable(driver)) {
        state = DBState::ERROR_NO_DRIVER;
        qCritical() << "Driver" << driver << "is not available";
        return false;
    }

    if (!setupWorkspace()) {
        state = DBState::ERROR_WORKSPACE;
        qCritical() << "Workspace setup failed";
        return false;
    }

    database.reset(new QSqlDatabase {QSqlDatabase::addDatabase(driver)});
    database->setDatabaseName(dbPath);
    qDebug() << "DatabaseName " << database->databaseName();

    if (!database->open()) {
        state = DBState::ERROR_OPENING;
        qCritical() << "Error in opening DB" << database->databaseName()
                    << "reason" << database->lastError().text();
        return false;
    }

    return setupTables();
}

bool ConnectionManager::ConnectionManagerPrivate::setupWorkspace()
{
#ifdef BUILD_TESTS
    const QString databaseName {"testDB"};
#else
    const QString databaseName {"registry"}; // WARNING : make dynamically
#endif

    const QString location {QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)};
    const QString fullPath {location + "/" + databaseName};
    dbPath = fullPath;

    QDir dbDirectory {location};
    if (!dbDirectory.exists()) {
        const bool creationResult {dbDirectory.mkdir(location)};
        qInfo() << "DB directory doesn't exist , creating result: " << creationResult;
    }

    qDebug() << "Data path: " << fullPath;
    return dbDirectory.exists();
}

bool ConnectionManager::ConnectionManagerPrivate::setupTables()
{
    bool result {true};

    std::vector<QSqlQuery> creationQueries {
        QSqlQuery {
            "CREATE TABLE IF NOT EXISTS '%1' ("
            "   'id'        INTEGER PRIMARY KEY AUTOINCREMENT, "
            "   'language'  CHAR(50), "
            "   'type'      CHAR(50), "
            "   'region'    CHAR(50) "
            ")"
        },
        QSqlQuery {
            "CREATE TABLE IF NOT EXISTS '%1' ("
            "   'id'                INTEGER PRIMARY KEY AUTOINCREMENT, "
            "   'name'              CHAR(200) NOT NULL, "
            "   'description'       TEXT, "
            "   'abbreviation'      CHAR(50), "
            "   'information'       TEXT, "
            "   'language'          CHAR(50), "
            "   'language_show'     CHAR(50), "
            "   'update'            TEXT, "
            "   'urls'              TEXT, "
            "   'comment'           TEXT, "
            "   'size'              NUMERIC NOT NULL, "
            "   'region'            TEXT, "
            "   'default_download'  NUMERIC DEFAULT 0, "
            "   'hidden'            NUMERIC DEFAULT 0, "
            "   'copyright'         TEXT, "
            "   '%2_id'             NUMERIC NOT NULL, "
            "FOREIGN KEY ('%2_id')  REFERENCES %2(id)"
            ")"
        }
    };

    for (auto & query : creationQueries)
    {
        if (!query.exec())
        {
            result = false;
            state = DBState::ERROR_TABLES;
            qWarning() << "Table creation failed. Reason:"
                        << query.lastError().text() << ". Query: " << query.lastQuery();
        }
        else
        {
            qInfo() << "Table successfully created. Query: " << query.lastQuery();
        }
    }

    return result;
}

};
