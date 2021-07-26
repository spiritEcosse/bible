#ifndef MODELMODULE_H
#define MODELMODULE_H

#include "modelupdate.h"

namespace modules {

    namespace tests
    {
       class tst_ModelModule;
    }

    class ModelModule : public ModelUpdate<Module>
    {
        Q_OBJECT
    public:
        enum ModuleRoles {
            Name = 0,
            Description = 1,
            Abbreviation = 2,
            Size = 3,
            LanguageShow = 4,
            AdditionalInfo = 5,
            DateUpdate = 6,
            Hidden = 7,
            DefaultDownload = 8,
            Id = 9,
            Downloading = 10,
        };

        ModelModule();
        ModelModule(int idGroupModules, const QString& needle = "");
        ~ModelModule();
        static void registerMe();
        Q_INVOKABLE int countAll();
        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
        void updateObjects();
        void search();
        inline const QString getNameJson() override { return QString("downloads"); };
    private:
        friend class tests::tst_ModelModule;
        int m_idGroupModules = 0;
        QString m_needle = "";
    };

}

#endif // MODELMODULE_H
