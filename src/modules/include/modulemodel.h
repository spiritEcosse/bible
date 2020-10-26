#ifndef MODULESMODEL_H
#define MODULESMODEL_H

#include <QAbstractListModel>
#include "module.h"
#include "db.h"


namespace modules {

    class ModuleModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        ModuleModel();
        virtual ~ModuleModel();

        int rowCount(const QModelIndex& parent = {}) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;
    //    virtual void init();
    //    virtual bool createTable();
    //    virtual bool execLastError(const QString& query);
    private:
        std::shared_ptr<db::Db<Module>> m_db;
        std::vector<Module> m_modules;

    //    std::vector<Modules> m_modules;
    //    friend class ModulesGroupModelTest;
    //    friend class ModulesModelTest;
    //    FRIEND_TEST(ModulesModelTest, createTable);
    //    QString tableNameString = "modules";
    //    QString tableNameGroup = "modules_group";
    };

}

#endif // MODULESMODEL_H
