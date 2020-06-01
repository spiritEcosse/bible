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
    Registry registry;
private slots:
    void makeTransform(const QJsonArray& object);
//    std::vector<ModulesGroups> transform;
};

#endif // MODULESGROUPSTRANSFORM_H
