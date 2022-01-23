#ifndef MODELMODULE_H
#define MODELMODULE_H

#include "modelupdate.h"
#include "curlmulti.h"
#include "modelhost.h"
#include <memory>
#include <QThread>

namespace modules {

    using namespace netmanager;
    using namespace sqlite_orm;

    namespace tests
    {
       class tst_ModelModule;
    }
    using Selected = decltype(std::vector<std::tuple<QString>>());
    using Downloaded = decltype(std::vector<std::tuple<QString>>());

    class Worker : public QObject, public BaseModel<Module>
    {
        Q_OBJECT
    public :
        Worker(QObject *parent = nullptr);
        ~Worker();
    private:
        friend class tests::tst_ModelModule;

        std::unique_ptr<CurlMulti> m_multi;
        std::unique_ptr<ModelHost> m_modelHost;
        int index = 0;

        virtual std::vector<QString> makeUrls(const Downloaded& downloaded) const;
        virtual bool bulkUpdateDownloaded(const Downloaded& downloaded, bool value = true);
        virtual bool unarchiveModules(const Downloaded& downloaded);
    public slots:
        virtual void deleteFiles(const Downloaded &downloaded);
        virtual bool startDownloadModules(const Downloaded& downloaded);
    private slots:
        virtual void updateSuccessfullyDownloaded();
        virtual void retryFailedDownloaded();
    signals:
        void deleteCompleted();
        bool downloadCompleted();
    };

    class ModelModule : public ModelUpdate<Module>
    {
        Q_OBJECT
        QThread workerThread;
        Q_PROPERTY(QVariantList downloaded READ getDownloaded NOTIFY changeDownloaded)
        Q_PROPERTY(QVariantList selected READ getSelected NOTIFY changeSelected)
        Q_PROPERTY(bool deleteCompleted READ getDeleteCompleted NOTIFY changeDeleteCompleted)
        Q_PROPERTY(bool downloadCompleted READ getDownloadCompleted NOTIFY changeDownloadCompleted)
    private:
        friend class tests::tst_ModelModule;
        int m_idGroupModules = 0;
        QString m_needle = "";
        Selected m_selectedBackup;
        Downloaded m_downloadedBackup;
        QVariantList m_downloaded;
        QVariantList m_selected;
        bool m_deleteCompleted = true;
        bool m_downloadCompleted = true;
        std::unique_ptr<Worker> m_worker;
        virtual Downloaded transformFromQVariant(const QVariantList& downloaded) const;
        virtual void retrieveDownloaded();
        virtual void retrieveSelecteded();
        virtual QVariantList getDownloaded();
        virtual QVariantList getSelected();
        virtual bool getDeleteCompleted();
        virtual bool getDownloadCompleted();
        virtual bool setDeleteCompleted(bool value);
        virtual bool setDownloadCompleted(bool value);
        virtual bool bulkUpdateDownloaded(const Downloaded& downloaded, bool value = true);

        template<class ColumnIn, class Array, class ColumnC, class ValueC, class ... Columns>
        inline auto getActualDataByField(const ColumnIn& columnIn, const Array& array, const ColumnC& columnC, const ValueC &valueC, Columns ... moduleColumns) const
        {
            return m_db->storage->select(
                        columns(std::forward<Columns>(moduleColumns)...),
                        where(in(columnIn, array) and c(columnC) == valueC)
                    );
        }

        template<class Column, class ... Columns>
        inline auto getDataByFieldTrue(const Column& column, Columns ... moduleColumns) const
        {
            return m_db->storage->select(
                        columns(std::forward<Columns>(moduleColumns)...),
                        where(c(column) == true)
                    );
        }

        template<class ColumnAssign, class ValueAssign, class ColumnC, class Value>
        inline void updateAllC(const ColumnAssign& columnAssign, const ValueAssign &valueAssign,
                              const ColumnC& columnC, const Value& value) const
        {
            m_db->storage->update_all(
                        set(assign(columnAssign, valueAssign)),
                        where(c(columnC) == value));
        }
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
            Book = 11,
        };

        ModelModule(QObject *parent = nullptr);
        ModelModule(int idGroupModules, const QString& needle = "");
        ~ModelModule();
        static void registerMe();
        Q_INVOKABLE int countActive();
        Q_INVOKABLE void init();
        Q_INVOKABLE void activateModule(int id) const;
        Q_INVOKABLE void updateSelected(int id, bool value) const;
        Q_INVOKABLE void updateSelectedBulk(const QVariantList& ids) const;
        Q_INVOKABLE virtual void downloadModules(const QVariantList& downloaded);
        Q_INVOKABLE virtual void deleteModules(const QVariantList& downloaded);
        Q_INVOKABLE void updateDownloaded(int id, bool value) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QHash<int, QByteArray> roleNames() const;
        void updateObjects();
        Q_INVOKABLE virtual void updateObjectsDownloaded();
        Q_INVOKABLE virtual void updateObjectsActive();
        void search();
        virtual const QString getNameJson();
    public slots:
        void getExtraFieldsFromDb();
    private slots:
        virtual void saveExtraFieldsToDb();
        virtual void postDeleteFiles();
        virtual void postDownloaded();
    signals:
        void changeDownloaded();
        void changeSelected();
        void changeDeleteCompleted();
        void changeDownloadCompleted();
        void bulkUpdatedDownloaded();
        bool startDeleteFiles(const Downloaded &downloaded);
        bool startDownloadModules(const Downloaded& downloaded);
    };

}

#endif // MODELMODULE_H
