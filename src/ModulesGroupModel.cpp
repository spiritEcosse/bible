#include "ModulesGroupModel.h"

static void createTable(QString const tableName)
{
    if (!QSqlDatabase::database().tables().contains(tableName)) {
        QSqlQuery query;
        QString sql;

        sql = QString(
                    "CREATE TABLE IF NOT EXISTS '%1' ("
                    "   'id'      INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "   'title'   CHAR(40) "
                    ")"
                    ).arg(tableName);

        if (!query.exec(sql)) {
            qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
        }
    }
}

ModulesGroupModel::ModulesGroupModel(QObject *parent)
    : QSqlTableModel(parent)
{
    createTable(tableName());
    setTable(tableName());
    select();
    registry.setFileName("registry.json");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

ModulesGroupModel::~ModulesGroupModel()
{
}

QString ModulesGroupModel::tableName() const
{
    return "modules_group";
}

void ModulesGroupModel::decompressRegistry()
{
    registryArchive.setFileName(manager.fileNames.last());
    QString registryName = JlCompress::extractFile(registryArchive.fileName(), registry.fileName());
    QFileInfo fileInfo(registryName);

    if (fileInfo.fileName() == registry.fileName())
        emit decompressSuccess();
}

void ModulesGroupModel::updateModules()
{
    manager.append(urlRegistry);
    connect(&manager, SIGNAL (successfully()), SLOT (decompressRegistry()));
    connect(this, SIGNAL (decompressSuccess()), SLOT (updateTable()));

    setCountOldRows();

    connect(this, SIGNAL (updateTableSuccess()), SLOT (removeOldRows()));
    connect(this, SIGNAL (updateTableSuccess()), SLOT (removeRegistryFile()));
}

void ModulesGroupModel::setCountOldRows()
{
    QSqlQuery query;
    query.exec(QString("SELECT COUNT(*) as count FROM %1").arg(tableName()));
    query.first();
    countOldRows = query.value("count").toInt();
}

void ModulesGroupModel::removeOldRows()
{
    removeRows(0, countOldRows);

    if (!submitAll()) {
        qWarning() << "Failed to remove rows: " << lastError().text();
    } else {
        emit removeOldRowsSuccess();
    }
}

void ModulesGroupModel::removeRegistryFile()
{
    if (registryArchive.remove()) {
        emit removeRegistryFileSuccess();
    }
}

void ModulesGroupModel::updateTable()
{
    if (!registry.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(registry.readAll(), &jsonError);
    registry.close();

    if(jsonError.error != QJsonParseError::NoError)
        return;

    QJsonArray downloads = document.object().value("downloads").toArray();
    newRows(downloads);
}

void ModulesGroupModel::newRows(QJsonArray &downloads)
{
    foreach(const QJsonValue &jsonValue, downloads) {
        QJsonObject jsonObject = jsonValue.toObject();
        QSqlRecord newRecord = record();
        newRecord.setValue("title", jsonObject.value("fil").toString());
//        newRecord.setValue("name", jsonObject.value("fil").toString());
//        newRecord.setValue("description", jsonObject.value("des").toString());
//        newRecord.setValue("abbreviation", jsonObject.value("abr").toString());
//        newRecord.setValue("information", jsonObject.value("inf").toString());
//        newRecord.setValue("language", jsonObject.value("lng").toString());
//        newRecord.setValue("language_show", jsonObject.value("aln").toString());
//        newRecord.setValue("update", jsonObject.value("upd").toString());
//        newRecord.setValue("urls", jsonObject.value("url").toString());
//        newRecord.setValue("comment", jsonObject.value("cmt").toString());
//        newRecord.setValue("size", correctSize(jsonObject.value("siz").toString()));
//        newRecord.setValue("region", jsonObject.value("reg").toString());
//        newRecord.setValue("default_download", jsonObject.value("def").toInt());
//        newRecord.setValue("hidden", jsonObject.value("hid").toInt());
//        newRecord.setValue("copyright", jsonObject.value("lic").toString());
        insertRecord(-1, newRecord);
    }

// This allows transactions to be rolled back and resubmitted without losing data.
    if (submitAll()) {
        emit updateTableSuccess();
    } else {
        qWarning() << "Failed to add new row: " << lastError().text();
    }
}

int ModulesGroupModel::correctSize(const QString &str) const
{
    QRegularExpression re("^([+-]?\\d*\\.?\\d+)(\\w{1})*$", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(str);
    double size = 0;
    QStringList dimensions = {"K", "M", "G"};

    if (match.hasMatch()) {
        size = match.captured(1).toDouble();
        QString dimension = match.captured(2).toUpper();
        size *= qPow(1024, dimensions.indexOf(dimension) + 1);
    }
//ToDo replace on formattedDataSize
    return size;
}

void ModulesGroupModel::checkAvailabilityNewModules()
{
    manager.append(urlRegistryInfo);
    connect(&manager, SIGNAL (successfully()), this, SLOT (compareVersions()));
}

void ModulesGroupModel::compareVersions()
{
    QFile registry_json;
    registry_json.setFileName(manager.fileNames.last());
    if (!registry_json.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(registry_json.readAll(), &jsonError);
    registry_json.close();

    if(jsonError.error != QJsonParseError::NoError)
        return;

    int version = document.object().value("version").toInt();
    QSettings settings;
    bool newModules = version > settings.value("modulesVersion").toInt();

    if (newModules) {
        settings.setValue("modulesVersion", version);
    }

    emit availabilityNewModules(newModules);
}

QString ModulesGroupModel::correctTitle(const QString &name, const QString &language, const QString &region) const
{
    QRegularExpression re(MODULES_SPLIT_NAME);
    QRegularExpressionMatch match = re.match(name);
    QString section;
    QLocale locale = language;
    QStringList sectionList;

    if (match.hasMatch()) {
        sectionList.append(match.captured(2));
    }

    if (!locale.nativeLanguageName().isEmpty()) {
        sectionList.append(locale.nativeLanguageName());
    }

    if (sectionList.empty() && !region.isEmpty()) {
        sectionList.append(region);
    }

    if (sectionList.empty()) {
        sectionList.append(language);
    }

    section = sectionList.join(", ").trimmed();

    if (!section.isEmpty()) {
        section[0] = section[0].toUpper();
    }
    return section;
}

QVariant ModulesGroupModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray> ModulesGroupModel::roleNames() const {
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "title";
    return names;
}
