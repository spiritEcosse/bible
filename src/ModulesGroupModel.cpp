#include "ModulesGroupModel.h"

ModulesGroupModel::ModulesGroupModel(QObject *parent)
    : QSqlTableModel(parent), countOldRows(0)
{
    init();
} // FIXME: test downloadedCount

void ModulesGroupModel::decompressRegistry()
{
    registryArchive.setFileName(manager->fileNames->last());
    QString registryName = JlCompress::extractFile(registryArchive.fileName(), registry->fileName());
    QFileInfo fileInfo(registryName);

    if (fileInfo.fileName() == registry->fileName()) {
        emit decompressSuccess();
    }
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
    if (!registry->open(QIODevice::ReadOnly | QIODevice::Text))
        return ;

    QJsonDocument document = qJsonDocument->fromJson(
                registry->readAll(), qJsonParserError);
    registry->close();

    if (qJsonParserError->error != QJsonParseError::NoError)
        return;

    QJsonArray data = document.object().value(QString("downloads")).toArray();
    newRows(data);
}

void ModulesGroupModel::compareVersions()
{
    registryVersion->setFileName(manager->fileNames->last());

    if ( !registryVersion->open(QIODevice::ReadOnly | QIODevice::Text) )
        return ;

    QJsonDocument document = qJsonDocument->fromJson(registryVersion->readAll(), qJsonParserError);
    registryVersion->close();

    if ( qJsonParserError->error != QJsonParseError::NoError ) // WARNING : test this
        return;

    int version = document.object().value("version").toInt();
    bool newModules = version > qSettings->value("modulesVersion").toInt();

    if (newModules) {
        qSettings->setValue("modulesVersion", version);
    }

    emit availabilityNewModules(newModules);
}

void ModulesGroupModel::init()
{
    createTable();
    setTable(tableNameString);
    select();
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    registry->setFileName("registry.json");
}

bool ModulesGroupModel::execLastError(const QString& query)
{
    if (!query_->exec(query))
    {
        qWarning() << query << query_->lastError().text();
        // qPrintable(query_->lastError().text());
        return false;
    }
    return true;
}

bool ModulesGroupModel::createTable()
{
    if ( !database().tables().contains(tableNameString) ) {
        return execLastError(qStringSql->arg(tableNameString));
    }
    return false;
}

void ModulesGroupModel::updateModules()
{
    manager->append(urlRegistry->fromEncoded(qQByteArray->fromBase64(registryBase64)));
    connect(manager, SIGNAL (successfully()), SLOT (decompressRegistry())); // WARNING : add connect to test
    connect(this, SIGNAL (decompressSuccess()), SLOT (updateTable())); // WARNING : add connect to test

    setCountOldRows();

    connect(this, SIGNAL (updateTableSuccess()), SLOT (removeOldRows())); // WARNING : add connect to test
    connect(this, SIGNAL (updateTableSuccess()), SLOT (removeRegistryFile())); // WARNING : add connect to test
}

void ModulesGroupModel::setCountOldRows()
{
    execLastError(qStringSelectSql->arg(tableNameString));
    query_->first();
    countOldRows = query_->value(QString("count")).toInt();
}

void ModulesGroupModel::newRows(QJsonArray &downloads)
{
    int lastId = 0;
    foreach(const QJsonValue &jsonValue, downloads) {
        QJsonObject jsonObject = jsonValue.toObject();
        QSqlRecord newRecord = record();
        QMap<QString, QString> group = makeGroup(
                    jsonObject.value("fil").toString(),
                    jsonObject.value("lng").toString(),
                    jsonObject.value("reg").toString());

        QMap<QString, QString>::const_iterator it = group.constBegin();
        while (it != group.constEnd()) {
            newRecord.setValue(it.key(), it.value());
            ++it;
        }
        QSqlRecord modulesModelRecord = modulesModel->record();
        modulesModelRecord.setValue("name", jsonObject.value("fil").toString());
        modulesModelRecord.setValue("description", jsonObject.value("des").toString());
        modulesModelRecord.setValue("abbreviation", jsonObject.value("abr").toString());
        modulesModelRecord.setValue("information", jsonObject.value("inf").toString());
        modulesModelRecord.setValue("language", jsonObject.value("lng").toString());
        modulesModelRecord.setValue("language_show", jsonObject.value("aln").toString());
        modulesModelRecord.setValue("update", jsonObject.value("upd").toString());
        modulesModelRecord.setValue("urls", jsonObject.value("url").toString());
        modulesModelRecord.setValue("comment", jsonObject.value("cmt").toString());
        modulesModelRecord.setValue("size", correctSize(jsonObject.value("siz").toString()));
        modulesModelRecord.setValue("region", jsonObject.value("reg").toString());
        modulesModelRecord.setValue("default_download", jsonObject.value("def").toInt());
        modulesModelRecord.setValue("hidden", jsonObject.value("hid").toInt());
        modulesModelRecord.setValue("copyright", jsonObject.value("lic").toString());
        modulesModelRecord.setValue(QString("%1_id").arg(tableNameString), ++lastId);
        insertRecord(-1, newRecord); // FIXME: add to this lastError().text()
        modulesModel->insertRecord(-1, modulesModelRecord);
    }

// This allows transactions to be rolled back and resubmitted without losing data.
    if (submitAll() && modulesModel->submitAll()) {
        emit updateTableSuccess();
    } else {
        qWarning() << "Failed to add new row: " << lastError().text();
    }
}

void ModulesGroupModel::checkAvailabilityNewModules()
{
    manager->append(urlRegistryInfo->fromEncoded(qQByteArray->fromBase64(registryInfoBase64)));
    connect(manager, SIGNAL (successfully()), this, SLOT (compareVersions()));
}

QMap<QString, QString>
ModulesGroupModel
::makeGroup(const QString &name, const QString &language, const QString &region) const
{
    QMap<QString, QString> group;
    QRegularExpression re(MODULES_SPLIT_NAME);
    QRegularExpressionMatch match = re.match(name);
    QLocale locale = language;

    if (match.hasMatch()) {
        group["type"] = match.captured(2);
    }

    if (!locale.nativeLanguageName().isEmpty()) {
        group["language"] = language;
    }

    if (group.empty() && !region.isEmpty()) {
        group["region"] = region;
    }

    if (group.empty()) {
        group["language"] = language;
    }
    return group;
}

QString ModulesGroupModel
::correctTitle(const QString &name, const QString &language, const QString &region) const
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

QVariant ModulesGroupModel
::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray>
ModulesGroupModel
::roleNames() const {
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "title";
    return names;
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
//WARNING: replace on formattedDataSize
    return size;
}
