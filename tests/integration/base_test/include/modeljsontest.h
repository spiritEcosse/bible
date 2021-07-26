#ifndef MODELJSONTEST_H
#define MODELJSONTEST_H

#include <QtTest>
#include "basetest.h"

namespace tests {

    template <class T, class O>
    class ModelJsonTest : public BaseTest<T, O>
    {
    public:
        ModelJsonTest();
        ~ModelJsonTest();

    protected:
        void initTestCase() override;
        void cleanupTestCase() override;
        QFile fileRegistry { "registry.json" };
        const QFile fileRegistryArchive { "registry.zip" };
        QFile fileRegistryInfo { "registry_info.json" };

        virtual void update();
        virtual void transform_data();
        virtual void transform();

        virtual QJsonDocument helperGetDocument();
        virtual QJsonDocument helperGetInvalidDocument() const;
        virtual std::vector<std::shared_ptr<T>> helperGetObjects() const = 0;
        virtual std::vector<std::unique_ptr<T>> helperGetObjectsUnique() const = 0;

        using ModelShared = decltype(std::shared_ptr<T>());
        using ModelUnique = decltype(std::unique_ptr<T>());
    };

}

#endif // MODELJSONTEST_H
