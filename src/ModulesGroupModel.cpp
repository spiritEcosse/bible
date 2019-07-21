//#include "ModulesGroupModel.h"
//#include "../tests/mock_iqsqldatabase.h"
//#include "../tests/mock_iqsqlquery.h"

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::decompressRegistry()
//{
////    registryArchive.setFileName(manager.fileNames.last());
////    QString registryName = JlCompress::extractFile(registryArchive.fileName(), registry.fileName());
////    QFileInfo fileInfo(registryName);

////    if (fileInfo.fileName() == registry.fileName())
////        emit decompressSuccess();
//}

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::removeOldRows()
//{
////    removeRows(0, countOldRows);

////    if (!submitAll()) {
////        qWarning() << "Failed to remove rows: " << lastError().text();
////    } else {
////        emit removeOldRowsSuccess();
////    }
//}

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::removeRegistryFile()
//{
////    if (registryArchive.remove()) {
////        emit removeRegistryFileSuccess();
////    }
//}

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::updateTable()
//{
////    if (!registry.open(QIODevice::ReadOnly | QIODevice::Text))
////        return ;

////    QJsonParseError jsonError;
////    QJsonDocument document = QJsonDocument::fromJson(registry.readAll(), &jsonError);
////    registry.close();

////    if(jsonError.error != QJsonParseError::NoError)
////        return;

////    QJsonArray downloads = document.object().value("downloads").toArray();
////    newRows(downloads);
//}

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::compareVersions()
//{
////    QFile registry_json;
////    registry_json.setFileName(manager.fileNames.last());
////    if (!registry_json.open(QIODevice::ReadOnly | QIODevice::Text))
////        return ;

////    QJsonParseError jsonError;
////    QJsonDocument document = QJsonDocument::fromJson(registry_json.readAll(), &jsonError);
////    registry_json.close();

////    if(jsonError.error != QJsonParseError::NoError)
////        return;

////    int version = document.object().value("version").toInt();
////    QSettings settings;
////    bool newModules = version > settings.value("modulesVersion").toInt();

////    if (newModules) {
////        settings.setValue("modulesVersion", version);
////    }

////    emit availabilityNewModules(newModules);
//}

//template <class QSqlDatabase, class QSqlQuery>
//ModulesGroupModel<QSqlDatabase, QSqlQuery>::ModulesGroupModel(QSqlDatabase &db, QObject *parent)
//    : QSqlTableModel(parent, db), db_(&db) {}

//template <class QSqlDatabase, class QSqlQuery>
//ModulesGroupModel<QSqlDatabase, QSqlQuery>::~ModulesGroupModel()
//{
//}

//template <class QSqlDatabase, class QSqlQuery>
//QSqlQuery& ModulesGroupModel<QSqlDatabase, QSqlQuery>::query() const
//{
//    QSqlQuery q;
//    return q;
//}

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::init()
//{
//    setTable("modules_group");
//    query_ = &query();
//    createTable("modules_group");
//    select();
////    setEditStrategy(QSqlTableModel::OnManualSubmit);
////    registry.setFileName("registry.json");
//}

//template <class QSqlDatabase, class QSqlQuery>
//bool ModulesGroupModel<QSqlDatabase, QSqlQuery>::execLastError(const QString& query)
//{
//    if (!query_->exec(query))
//    {
//        qPrintable(query_->lastError().text());
//        return false;
//    }
//    return true;
//}

//template <class QSqlDatabase, class QSqlQuery>
//bool ModulesGroupModel<QSqlDatabase, QSqlQuery>::createTable(const QString &tableName)
//{
//    if ( !db_->tables().contains(tableName) ) {
//        QString sql = QString(
//                    "CREATE TABLE IF NOT EXISTS '%1' ("
//                    "   'id'        INTEGER PRIMARY KEY AUTOINCREMENT, "
//                    "   'language'  CHAR(50), "
//                    "   'type'      CHAR(50), "
//                    "   'region'    CHAR(50) "
//                    ).arg(tableName);
//        return execLastError(sql);
//    }
//    return false;
//}

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::updateModules()
//{
////    manager.append(urlRegistry);
////    connect(&manager, SIGNAL (successfully()), SLOT (decompressRegistry()));
////    connect(this, SIGNAL (decompressSuccess()), SLOT (updateTable()));

////    setCountOldRows();

////    connect(this, SIGNAL (updateTableSuccess()), SLOT (removeOldRows()));
////    connect(this, SIGNAL (updateTableSuccess()), SLOT (removeRegistryFile()));
//}

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::setCountOldRows()
//{
////    QSqlQuery query;
////    query.exec(QString("SELECT COUNT(*) as count FROM %1").arg(tableName()));
////    query.first();
////    countOldRows = query.value("count").toInt();
//}

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::newRows(QJsonArray &downloads)
//{
//    foreach(const QJsonValue &jsonValue, downloads) {
//        QJsonObject jsonObject = jsonValue.toObject();
//        QSqlRecord newRecord = record();
//        QMap<QString, QString> group = makeGroup(
//                    jsonObject.value("fil").toString(),
//                    jsonObject.value("lng").toString(),
//                    jsonObject.value("reg").toString());

//        QMap<QString, QString>::const_iterator it = group.constBegin();
//        while (it != group.constEnd()) {
//            newRecord.setValue(it.key(), it.value());
//            ++it;
//        }

////        newRecord.setValue("name", jsonObject.value("fil").toString());
////        newRecord.setValue("description", jsonObject.value("des").toString());
////        newRecord.setValue("abbreviation", jsonObject.value("abr").toString());
////        newRecord.setValue("information", jsonObject.value("inf").toString());
////        newRecord.setValue("language", jsonObject.value("lng").toString());
////        newRecord.setValue("language_show", jsonObject.value("aln").toString());
////        newRecord.setValue("update", jsonObject.value("upd").toString());
////        newRecord.setValue("urls", jsonObject.value("url").toString());
////        newRecord.setValue("comment", jsonObject.value("cmt").toString());
////        newRecord.setValue("size", correctSize(jsonObject.value("siz").toString()));
////        newRecord.setValue("region", jsonObject.value("reg").toString());
////        newRecord.setValue("default_download", jsonObject.value("def").toInt());
////        newRecord.setValue("hidden", jsonObject.value("hid").toInt());
////        newRecord.setValue("copyright", jsonObject.value("lic").toString());
//        insertRecord(-1, newRecord);
//    }

//// This allows transactions to be rolled back and resubmitted without losing data.
//    if (submitAll()) {
////        emit updateTableSuccess();
//    } else {
////        qWarning() << "Failed to add new row: " << lastError().text();
//    }
//}

//template <class QSqlDatabase, class QSqlQuery>
//void ModulesGroupModel<QSqlDatabase, QSqlQuery>::checkAvailabilityNewModules()
//{
////    manager.append(urlRegistryInfo);
////    connect(&manager, SIGNAL (successfully()), this, SLOT (compareVersions()));
//}

//template <class QSqlDatabase, class QSqlQuery>
//QMap<QString, QString>
//ModulesGroupModel<QSqlDatabase, QSqlQuery>
//::makeGroup(const QString &name, const QString &language, const QString &region) const
//{
//    QMap<QString, QString> group;
//    QRegularExpression re(MODULES_SPLIT_NAME);
//    QRegularExpressionMatch match = re.match(name);
//    QLocale locale = language;

//    if (match.hasMatch()) {
//        group["type"] = match.captured(2);
//    }

//    if (!locale.nativeLanguageName().isEmpty()) {
//        group["language"] = language;
//    }

//    if (group.empty() && !region.isEmpty()) {
//        group["region"] = region;
//    }

//    if (group.empty()) {
//        group["language"] = language;
//    }

//    return group;
//}

//template <class QSqlDatabase, class QSqlQuery>
//QString ModulesGroupModel<QSqlDatabase, QSqlQuery>
//::correctTitle(const QString &name, const QString &language, const QString &region) const
//{
//    QRegularExpression re(MODULES_SPLIT_NAME);
//    QRegularExpressionMatch match = re.match(name);
//    QString section;
//    QLocale locale = language;
//    QStringList sectionList;

//    if (match.hasMatch()) {
//        sectionList.append(match.captured(2));
//    }

//    if (!locale.nativeLanguageName().isEmpty()) {
//        sectionList.append(locale.nativeLanguageName());
//    }

//    if (sectionList.empty() && !region.isEmpty()) {
//        sectionList.append(region);
//    }

//    if (sectionList.empty()) {
//        sectionList.append(language);
//    }

//    section = sectionList.join(", ").trimmed();

//    if (!section.isEmpty()) {
//        section[0] = section[0].toUpper();
//    }
//    return section;
//}

//template <class QSqlDatabase, class QSqlQuery>
//QVariant ModulesGroupModel<QSqlDatabase, QSqlQuery>
//::data(const QModelIndex &index, int role) const
//{
//    if (role < Qt::UserRole) {
//        return QSqlTableModel::data(index, role);
//    }

//    const QSqlRecord sqlRecord = record(index.row());
//    return sqlRecord.value(role - Qt::UserRole);
//}

//template <class QSqlDatabase, class QSqlQuery>
//QHash<int, QByteArray>
//ModulesGroupModel<QSqlDatabase, QSqlQuery>
//::roleNames() const {
//    QHash<int, QByteArray> names;
//    names[Qt::UserRole] = "title";
//    return names;
//}

//template class ModulesGroupModel<QSqlDatabase, QSqlQuery>;
//template class ModulesGroupModel<MockIQSqlDatabase, MockIQSqlQuery>;
