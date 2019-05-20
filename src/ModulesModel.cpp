#include "ModulesModel.h"

static void createTable(QString const tableName)
{
    if (!QSqlDatabase::database().tables().contains(tableName)) {
        QSqlQuery query;
        QString sql;

        sql = QString(
                    "CREATE TABLE IF NOT EXISTS '%1' ("
                    "   'id'                INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "   'name'              CHAR(200) NOT NULL, "
                    "   'description'       TEXT, "
                    "   'abbreviation'      CHAR(50), "
                    "   'information'       TEXT, "
                    "   'language'          CHAR(50), "
                    "   'language_show'     CHAR(50), "
                    "   'update'            TEXT NOT NULL, "
                    "   'urls'              TEXT, "
                    "   'comment'           TEXT, "
                    "   'size'              NUMERIC NOT NULL, "
                    "   'region'            TEXT, "
                    "   'default_download'  NUMERIC DEFAULT 0, "
                    "   'hidden'            NUMERIC DEFAULT 0, "
                    "   'copyright'         TEXT "
                    ")"
                    ).arg(tableName);

        if (!query.exec(sql)) {
            qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
        }
    }
}

ModulesModel::ModulesModel(QObject *parent)
    : QSqlTableModel(parent)
{
    createTable(tableName());
    setTable(tableName());
    select();
    registry.setFileName("registry.json");
}

QString ModulesModel::tableName() const
{
    return "modules";
}

ModulesModel::~ModulesModel()
{
}

void ModulesModel::decompressRegistry()
{
    registryArchive.setFileName(manager.fileNames.last());
    QString registryName = JlCompress::extractFile(registryArchive.fileName(), registry.fileName());
    QFileInfo fileInfo(registryName);

    if (fileInfo.fileName() == registry.fileName())
        emit decompressSuccess();
}

void ModulesModel::updateModules()
{
    manager.append(urlRegistry);
    connect(&manager, SIGNAL (successfully()), SLOT (decompressRegistry()));
    connect(this, SIGNAL (decompressSuccess()), SLOT (updateTable()));
    connect(this, SIGNAL (updateTableSuccess()), SLOT (removeRegistryFile()));
}

void ModulesModel::removeRegistryFile()
{
    if (registryArchive.remove()) {
        emit removeRegistryFileSuccess();
    }
}

void ModulesModel::updateTable()
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
    emit updateTableSuccess();
}

void ModulesModel::newRows(QJsonArray &downloads)
{
    foreach(const QJsonValue &jsonValue, downloads) {
        QJsonObject jsonObject = jsonValue.toObject();
        QSqlRecord newRecord = record();
        newRecord.setValue("name", jsonObject.value("fil").toString());
        newRecord.setValue("description", jsonObject.value("des").toString());
        newRecord.setValue("abbreviation", jsonObject.value("abr").toString());
        newRecord.setValue("information", jsonObject.value("inf").toString());
        newRecord.setValue("language", jsonObject.value("lng").toString());
        newRecord.setValue("language_show", jsonObject.value("aln").toString());
        newRecord.setValue("update", jsonObject.value("upd").toString());
        newRecord.setValue("urls", jsonObject.value("url").toString());
        newRecord.setValue("comment", jsonObject.value("cmt").toString());
        newRecord.setValue("size", correctSize(jsonObject.value("siz").toString()));
        newRecord.setValue("region", jsonObject.value("reg").toString());
        newRecord.setValue("default_download", jsonObject.value("def").toInt());
        newRecord.setValue("hidden", jsonObject.value("hid").toInt());
        newRecord.setValue("copyright", jsonObject.value("lic").toString());

        if (!insertRecord(-1, newRecord)) {
            qWarning() << "Failed to add new row: " << lastError().text();
        }
    }
}

int ModulesModel::correctSize(const QString &str) const
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

void ModulesModel::checkAvailabilityNewModules()
{
    manager.append(urlRegistryInfo);
    connect(&manager, SIGNAL (successfully()), this, SLOT (compareVersions()));
}

void ModulesModel::compareVersions()
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

QString ModulesModel::section(const QString &name, const QString &language, const QString &region) const
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

//ModulesModel::parserJson(const QJsonValue &jsonValue) const
//{
//    QList<QString> urls;
//    QJsonObject jsonObject = jsonValue.toObject();
//    foreach(const QJsonValue & url, jsonObject.value("url").toArray())
//        urls.append(url.toString());

//    QList<QHash<QString, QString>> locale_desc;
//    foreach(const QJsonValue & val, jsonObject.value("lds").toArray()) {
//        QHash<QString, QString> hash;
//        hash["lng"] = val.toObject().value("lng").toString();
//        hash["des"] = val.toObject().value("des").toString();
//        locale_desc.append(hash);
//    }

//    return Module(
//                jsonObject.value("fil").toString(),
//                jsonObject.value("des").toString(),
//                jsonObject.value("abr").toString(),
//                QTextDocumentFragment::fromHtml(
//                    jsonObject.value("inf").toString()
//                    ).toPlainText(),
//                this->correctLanguage(jsonObject),
//                jsonObject.value("aln").toString(),
//                QDate::fromString(
//                    jsonObject.value("upd").toString(), "yyyy-MM-dd"
//                ),
//                urls,
//                locale_desc,
//                jsonObject.value("cmt").toString(),
//                this->correctSize(jsonObject.value("siz")),
//                jsonObject.value("reg").toString(),
//                jsonObject.value("def").toBool(),
//                jsonObject.value("hid").toBool(),
//                jsonObject.value("lic").toString()
//                );
//}

QVariant ModulesModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray> ModulesModel::roleNames() const {
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "updateDate";
    names[Qt::UserRole + 1] = "description";
    names[Qt::UserRole + 2] = "abbreviation";
    names[Qt::UserRole + 3] = "information";
    names[Qt::UserRole + 4] = "comment";
    names[Qt::UserRole + 5] = "size";
    names[Qt::UserRole + 6] = "language";
    names[Qt::UserRole + 7] = "section";
    return names;
}
