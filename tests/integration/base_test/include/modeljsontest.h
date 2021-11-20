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
        virtual std::vector<std::shared_ptr<T>> helperGetObjects() const override = 0;
        virtual std::vector<std::unique_ptr<T>> helperGetObjectsUnique() const override = 0;

        QFile fileRegistry { "registry.json" };
        const QFile fileRegistryArchive { "registry.zip" };
        QFile fileRegistryInfo { "registry_info.json" };

        virtual void update();
        virtual void transform_data();
        virtual void transform();
        virtual QJsonDocument helperGetDocument();
        virtual QJsonDocument helperGetInvalidDocument() const;

        using ModelShared = decltype(std::shared_ptr<T>());
        using ModelUnique = decltype(std::unique_ptr<T>());
    };

}

#endif // MODELJSONTEST_H
