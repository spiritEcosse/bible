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
        std::shared_ptr<db::Db<T>> m_db = db::Db<T>::getInstance();
        std::vector<T> helperSave() const;
        virtual void cleanTable();
        virtual std::vector<T> helperGetObjects() const;
        virtual void initTestCase();
        virtual void cleanupTestCase();
        virtual void update();
    };

}

#endif // BASE_H
