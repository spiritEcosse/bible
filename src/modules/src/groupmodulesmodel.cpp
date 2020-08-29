#include "groupmodulesmodel.h"
#if (QT_VERSION > QT_VERSION_CHECK(5, 6, 3))
#include <QQmlEngine>
#else
//#include <QtQuick>
#endif
#include <QDebug>

GroupModulesModel::GroupModulesModel()
{
    init();
}

GroupModulesModel::~GroupModulesModel() {}

void GroupModulesModel::registerMe(const std::string& moduleName)
{
//    qmlRegisterType<ModulesGroupsModel>(moduleName.c_str(), 1, 0, "ModulesGroupsModel");
}

void GroupModulesModel::init()
{
    if (!update()) {
        qWarning() << "Update groups failed!";
    }

    registry->setFileName("registry.json");
}

bool GroupModulesModel::update()
{
//    m_reader.reset(new ModulesGroupsReader());

//    bool requestResult {false};
//    std::vector<ModulesGroups> groupsResult;
//    std::tie(requestResult, groupsResult) = m_reader->requestModulesGroupsBrowse();

//    if (requestResult) {
//        m_groups.swap(groupsResult);
//        emit dataChanged(createIndex(0, 0), createIndex(m_groups.size(), 0));
//    }

//    return requestResult;
}

//void ModulesGroupsModel::updateTable()
//{
//    if (!registry->open(QIODevice::ReadOnly | QIODevice::Text))
//        return ;

//    QJsonDocument document = qJsonDocument->fromJson(
//                registry->readAll(), qJsonParserError);
//    registry->close();

//    if (qJsonParserError->error != QJsonParseError::NoError)
//        return;

//    QJsonArray data = document.object().value(QString("downloads")).toArray();
//    newRows(data);
//}


//bool ModulesGroupsModel::execLastError(const QString& query)
//{
//    if (!query_->exec(query))
//    {
//        qWarning() << query << query_->lastError().text();
//        // qPrintable(query_->lastError().text());
//        return false;
//    }
//    return true;
//}

void GroupModulesModel::newRows(QJsonArray &downloads)
{
//    int lastId = 0;
//    foreach(const QJsonValue &jsonValue, downloads) {
//        QJsonObject jsonObject = jsonValue.toObject();
////        QSqlRecord newRecord = record();
//        QMap<QString, QString> group = makeGroup(
//                    jsonObject.value("fil").toString(),
//                    jsonObject.value("lng").toString(),
//                    jsonObject.value("reg").toString());

//        QMap<QString, QString>::const_iterator it = group.constBegin();
//        while (it != group.constEnd()) {
////            newRecord.setValue(it.key(), it.value());
//            ++it;
//        }
//        QSqlRecord modulesModelRecord = modulesModel->record();
//        modulesModelRecord.setValue("name", jsonObject.value("fil").toString());
//        modulesModelRecord.setValue("description", jsonObject.value("des").toString());
//        modulesModelRecord.setValue("abbreviation", jsonObject.value("abr").toString());
//        modulesModelRecord.setValue("information", jsonObject.value("inf").toString());
//        modulesModelRecord.setValue("language", jsonObject.value("lng").toString());
//        modulesModelRecord.setValue("language_show", jsonObject.value("aln").toString());
//        modulesModelRecord.setValue("update", jsonObject.value("upd").toString());
//        modulesModelRecord.setValue("urls", jsonObject.value("url").toString());
//        modulesModelRecord.setValue("comment", jsonObject.value("cmt").toString());
//        modulesModelRecord.setValue("size", correctSize(jsonObject.value("siz").toString()));
//        modulesModelRecord.setValue("region", jsonObject.value("reg").toString());
//        modulesModelRecord.setValue("default_download", jsonObject.value("def").toInt());
//        modulesModelRecord.setValue("hidden", jsonObject.value("hid").toInt());
//        modulesModelRecord.setValue("copyright", jsonObject.value("lic").toString());
//        modulesModelRecord.setValue(QString("%1_id").arg(tableNameString), ++lastId);
////        insertRecord(-1, newRecord); // FIXME: add to this lastError().text()
//        modulesModel->insertRecord(-1, modulesModelRecord);
//    }

//// This allows transactions to be rolled back and resubmitted without losing data.
////    if (submitAll() && modulesModel->submitAll()) {
////        emit updateTableSuccess();
////    } else {
////        qWarning() << "Failed to add new row: " << lastError().text();
////    }
}

//QMap<QString, QString>
//ModulesGroupsModel
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

QVariant GroupModulesModel
::data(const QModelIndex &index, int role) const
{
//    if (role < Qt::UserRole) {
//        return QSqlTableModel::data(index, role);
//    }

//    const QSqlRecord sqlRecord = record(index.row());
//    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray>
GroupModulesModel::roleNames() const {
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "title";
    return names;
}

int GroupModulesModel::rowCount(const QModelIndex& /* parent */) const
{
//    return static_cast<int>(m_groups.size());
}
