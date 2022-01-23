#ifndef TST_MANAGERREGISTRY_H
#define TST_MANAGERREGISTRY_H

#include <QtTest>
#include "modeljsontest.h"
#include "managerregistry.h"

namespace modules {

    namespace tests {

        class tst_ManagerRegistry : public ::tests::ModelJsonTest<Registry, ModelRegistry>
        {
            Q_OBJECT

        public:
            tst_ManagerRegistry();
            ~tst_ManagerRegistry();

        private:
            void createFileRegistryInfo();
            void createFileRegistry(
                    const QJsonDocument& document = QJsonDocument {},
                    const QString& fileNameRegistry = "registry.json");
            void createFileRegistryArchive(
                    const QJsonDocument& document = QJsonDocument {},
                    const QString& fileNameArchive = "registry.zip",
                    const QString& fileNameRegistry = "registry.json");
            void setQSettings(int value = 0, QString key = "registryVersion");
            QJsonDocument helperGetInvalidDocument() const override;
            std::vector<RegistryShared> helperGetObjects() const override;
            std::vector<RegistryUnique> helperGetObjectsUnique() const override;

        private slots:
            void initTestCase() override;
            void cleanupTestCase() override;
            void download();
            void downloadManagerFailed();
            void downloadManagerFailedWithoutRecursion();
            void removeRegistry();
            void retrieveData_data();
            void retrieveData();
            void hasNewRegistry_data();
            void hasNewRegistry();
            void extractRegistry_data();
            void extractRegistry();
            void getDocument_data();
            void getDocument();
            void retrieveVersion();
            void retrieveDataInfo_data();
            void retrieveDataInfo();
            void checkNewVersion();
            void getVersion_data();
            void getVersion();
            void removeInfo();
            void setVersion_data();
            void setVersion();
            void getVersionFromCache_data();
            void getVersionFromCache();
            void tryOther_data();
            void tryOther();
        };
    }
}

#endif // TST_MANAGERREGISTRY_H
