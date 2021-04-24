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
        const QString pathFiles { "files" };
        const QString dirDownload = "download";
        QDir dir;
        std::shared_ptr<db::Db<T>> m_db;
        std::vector<std::shared_ptr<T>> helperSave(std::vector<std::shared_ptr<T>>&& entries = {});
        virtual void cleanTable();
        virtual std::vector<std::shared_ptr<T>> helperGetObjects() const;
        virtual std::vector<std::unique_ptr<T>> helperGetObjectsUnique() const;
        virtual void initTestCase();
        virtual void cleanupTestCase();
        virtual void update();
    };

}

#endif // BASE_H
