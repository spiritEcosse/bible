#ifndef MODULESGROUPSTRANSFORM_H
#define MODULESGROUPSTRANSFORM_H

#include <QObject>
#include "registry.h"

class QJsonDocument;

class ModulesGroupsFiller : public QObject
{
    Q_OBJECT
public:
    ModulesGroupsFiller();
private:
    friend class tst_ModulesGroupsFiller;
    Registry registry;
    std::string hashKey(const QJsonObject& obj);
private slots:
    void makeTransform(const QJsonArray& object);
//    std::vector<ModulesGroups> transform;
};

#endif // MODULESGROUPSTRANSFORM_H
