#include "ConnectionManager.h"
#include "dbtypes.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

#include <QDebug>

#include <QStandardPaths>
#include <QDir>

using namespace DBTypes;

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
    ConnectionManagerPrivate(const QString& nameDb = "user")
        : m_nameDb {nameDb} {}
    std::unique_ptr<QSqlDatabase, DBCloser> database;
    QString dbPath;
    bool isValid {true};
    DBState state {DBState::OK};
    QString m_nameDb;
    bool setup();
    bool setupWorkspace();
    bool setupTables();
};

ConnectionManager::ConnectionManager(const QString& nameDb)
    : m_d(new ConnectionManagerPrivate {nameDb})
{
    m_d->isValid = m_d->setup();
}

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

// WARNING : using chain : https://en.wikipedia.org/wiki/Variadic_template
bool ConnectionManager::ConnectionManagerPrivate::setup()
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

    if (!QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        database.reset(new QSqlDatabase {QSqlDatabase::addDatabase(driver)});
        database->setDatabaseName(dbPath);
    } else {
        database.reset(new QSqlDatabase {QSqlDatabase::database(QSqlDatabase::defaultConnection)});
        database->setDatabaseName(dbPath);
    }
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
    const QString databaseName {QString("%1.sqlite").arg(m_nameDb)}; // WARNING : make dynamically
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

    QString tableModules = "modules";
    QString tableModulesGroup = "modules_groups";

    std::vector<QSqlQuery> creationQueries {
        QSqlQuery {
            QString(
                        "CREATE TABLE IF NOT EXISTS '%1' ("
                        "   'id'        INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "   'language'  CHAR(4), "
                        "   'name'      CHAR(50), "
                        "   'region'    CHAR(50), "
                        "   UNIQUE (language, name, region) "
                        ")").arg(tableModulesGroup)
        },
        QSqlQuery {
            QString(
                        "CREATE TABLE IF NOT EXISTS '%1' ("
                        "   'id'                INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "   'name'              CHAR(200) NOT NULL, "
                        "   'description'       TEXT, "
                        "   'abbreviation'      CHAR(50), "
                        "   'information'       TEXT, "
                        "   'language_show'     CHAR(4), "
                        "   'update'            TEXT, "
                        //"   'urls'              TEXT, "
                        "   'comment'           TEXT, "
                        "   'size'              DOUBLE NOT NULL, "
                        "   'region'            CHAR(50), "
                        "   'default_download'  INTEGER DEFAULT 0, "
                        "   'hidden'            INTEGER DEFAULT 0, "
                        "   'copyright'         TEXT, "
                        "   '%2_id'             NUMERIC NOT NULL, "
                        "FOREIGN KEY ('%2_id')  REFERENCES %2(id)"
                        ")"
                        ).arg(tableModules, tableModulesGroup)
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
