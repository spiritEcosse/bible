#ifndef BASE_H
#define BASE_H

#include <QObject>
#include <QDir>
#include <QtTest>
#include "db.h"

namespace tests {

    template <class T, class O>
    class BaseTest : public QObject
    {
    public:
        BaseTest();
        ~BaseTest();
    protected:
        const size_t vectorSize = 3;
        const int version = 10;
        const QString strUrl { "file://" };
        const QString urlMask { "/%s.zip" };
        QString moduleName = "name.0";
        const QString pathFiles { "files" };
        const QString dirDownload = "download";
        QFile fileModuleArchive { QString("%1.zip").arg(moduleName) };
        QFile fileModuleArchiveInModules { QString("modules/%1.zip").arg(moduleName) };
        QDir folderModuleInModules { QString("modules/%1").arg(moduleName) };
        QDir dir;
        std::unique_ptr<db::Db<T>> m_db;
        std::vector<std::shared_ptr<T>> helperSave(std::vector<std::shared_ptr<T>>&& entries = {});
        virtual void initDb();
        virtual void cleanTable();
        virtual std::vector<std::shared_ptr<T>> helperGetObjects() const;
        virtual std::vector<std::unique_ptr<T>> helperGetObjectsUnique() const;
        virtual void initTestCase();
        virtual void cleanupTestCase();

        virtual void createFileModule();
        virtual void createFileModuleInModules();
        virtual void createFolderModuleInModules();
        virtual QString getModulePath() const;
        virtual QString getModuleFolderPath() const;
        virtual QString getModuleFilePath(const QFile &file = QFile("")) const;
        virtual QString makeUrlModuleHost() const;
        virtual QByteArray urlModuleToBase64() const;

        using ModelShared = decltype(std::shared_ptr<T>());
        using ModelUnique = decltype(std::unique_ptr<T>());
    };

}

#endif // BASE_H
