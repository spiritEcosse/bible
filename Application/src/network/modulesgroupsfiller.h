#ifndef MODULESGROUPSTRANSFORM_H
#define MODULESGROUPSTRANSFORM_H

#include <QObject>
#include "registry.h"
#include "modulesgroups.h"
#include <unordered_map>

// WARNING: replace this three on one class
struct MGKey {
    std::string name;
    std::string language;
    std::string region;
};

struct MGKeyHash {
    std::size_t operator()(const MGKey& mgKey) const
    {
        using namespace std;
        return ((hash<string>()(mgKey.name)
                 ^ (hash<string>()(mgKey.language) << 1)) >> 1)
                 ^ (hash<string>()(mgKey.region) << 1);
    }
};

struct MGKeyEqual {
    bool operator()(const MGKey& lhs, const MGKey& rhs) const
    {
        return lhs.name == rhs.name && lhs.language == rhs.language && lhs.region == rhs.region;
    }
};

class ModulesGroupsFiller : public QObject
{
    Q_OBJECT
public:
    ModulesGroupsFiller();

public slots:
    void downloadRegistry();

signals:
    void completed(std::unordered_map<MGKey, ModulesGroups, MGKeyHash, MGKeyEqual>);

private:
    friend class tst_ModulesGroupsFiller;
    std::unique_ptr<Registry> m_registry;
    std::unordered_map<MGKey, ModulesGroups, MGKeyHash, MGKeyEqual> fill(const QJsonArray& object);

private slots:
    void run(const QJsonArray& object);

};

#endif // MODULESGROUPSTRANSFORM_H
