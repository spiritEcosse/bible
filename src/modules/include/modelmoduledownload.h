#ifndef MODELMODULEDOWNLOAD_H
#define MODELMODULEDOWNLOAD_H

#include "model.h"

namespace modules {

    namespace tests
    {
       class tst_ModelModuleDownload;
    }

    class ModelModuleDownload : public Model<ModuleDownload>
    {
        Q_OBJECT
    public:
        enum Roles {
            ModuleIdRole = 0,
            GroupIdRole = 1,
            SelectingRole = 2,
            DownloadedRole = 3,
            DownloadedIdRole = 4,
        };

        ModelModuleDownload();
        ~ModelModuleDownload();

        static void registerMe();
        void updateObjects();
        void create(const QString& abbreviation, bool selecting, bool downloaded = false);
        void remove(int id);
        Q_INVOKABLE void clearObjects();
        Q_INVOKABLE void removeAllObjects();
        Q_INVOKABLE void crudWithSelecting(const QString& abbreviation, bool selecting);
        Q_INVOKABLE void crudWithDownloaded(const QString& abbreviation, bool downloaded);

        virtual QVariant data(const QModelIndex &index = {}, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;
    private:
        friend class tests::tst_ModelModuleDownload;
    };

}
#endif // MODELMODULEDOWNLOAD_H
