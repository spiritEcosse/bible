#include <QtTest>
#include "modelregistry.h"


namespace modules {

    namespace tests {

        class tst_ModelRegistry : public QObject {
            Q_OBJECT

        private:
            QJsonDocument helperGetDocument();
            std::shared_ptr<db::Db> m_db;

        public:
            tst_ModelRegistry();
            ~tst_ModelRegistry();

        private slots:
            void update();
        };

        //helpers

        QJsonDocument tst_ModelRegistry::helperGetDocument()
        {
            QJsonArray array;

            array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};
            array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};
            array << QJsonObject {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}};

            return QJsonDocument {
                QJsonObject {
                    { "registries",  array }
                }
            };
        }

        tst_ModelRegistry::tst_ModelRegistry()
            : m_db { db::Db::getInstance() }
        {
        }

        tst_ModelRegistry::~tst_ModelRegistry()
        {

        }

        // tests
        void tst_ModelRegistry::update()
        {
            ModelRegistry modelRegistry;
            QSignalSpy spyLast(&modelRegistry, &ModelRegistry::updateSuccess);

            m_db->storage->remove_all<Registry>();
            QCOMPARE(m_db->storage->count<Registry>(), 0);

            modelRegistry.update(helperGetDocument());
            QCOMPARE(m_db->storage->count<Registry>(), 3);

            QCOMPARE(spyLast.count(), 1);
        }

    }

}

QTEST_MAIN(modules::tests::tst_ModelRegistry)

#include "tst_modelregistry.moc"
