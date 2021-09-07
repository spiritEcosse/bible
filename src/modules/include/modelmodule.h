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
    private:
        friend class tests::tst_ModelModule;
        using Selected = decltype(std::vector<std::tuple<QString>>());
        using Downloaded = decltype(std::vector<std::tuple<QString>>());
        int m_idGroupModules = 0;
        QString m_needle = "";
        std::unique_ptr<Selected> selected;
        std::unique_ptr<Downloaded> downloaded;
        virtual void downloadModules(const std::unique_ptr<Downloaded> &downloaded) const;
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
        Q_INVOKABLE int countActive();
        Q_INVOKABLE void updateSelected(int id, bool selected) const;
        Q_INVOKABLE void updateSelectedBulk(const QVariantList& ids) const;
        Q_INVOKABLE void updateDownloaded(int id, bool downloaded) const;
        Q_INVOKABLE virtual const QVariant getExtraFields() const;
        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
        void updateObjects();
        void search();
        inline const QString getNameJson() override { return QString("downloads"); };
    public slots:
        void getExtraFieldsFromDb();
    private slots:
        void saveExtraFieldsToDb();
        void onReadyRead();
    };

}

#endif // MODELMODULE_H
