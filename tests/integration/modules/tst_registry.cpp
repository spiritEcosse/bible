#include <QtTest>
#include "registry.h"

Q_DECLARE_METATYPE(modules::Registry)

namespace modules {

    namespace tests {

        class tst_Registry : public QObject {
            Q_OBJECT

        public:
            tst_Registry();
            ~tst_Registry();

        private:
            Registry helperGetRegistry();

        private slots:
            void constructor_data();
            void constructor();
            void urlToQUrl();
            void infoUrlToQUrl();
        };

        tst_Registry::tst_Registry() {}

        tst_Registry::~tst_Registry() {}

        Registry tst_Registry::helperGetRegistry()
        {
            return Registry ("bGluazE=", "bGluazEx", 1, true);
        }

        void tst_Registry::constructor_data()
        {
            QTest::addColumn<QJsonObject>("object");
            QTest::addColumn<Registry>("registry");
            QTest::addColumn<bool>("except");

            QJsonObject data {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}, {"test", true}};
            QTest::newRow("valid data") << data << helperGetRegistry() << false;

            data = {
                {"url", "link1"},
                {"info_url", "link11"}
            };
            QTest::newRow("default fields") << data << Registry ("bGluazE=", "bGluazEx", 0, false) << false;

            data = {{"priority", 1}, {"info_url", "link11"}};
            QTest::newRow("invalid data: required m_url.") << data << Registry {} << true;

            data = {{"url", "link1"}, {"priority", 1}};
            QTest::newRow("invalid data: required m_infoUrl.") << data << Registry {} << true;
        }

        void tst_Registry::constructor()
        {
            QFETCH(QJsonObject, object);
            QFETCH(Registry, registry);
            QFETCH(bool, except);

            if (except) {
                QVERIFY_EXCEPTION_THROWN(Registry {object}, RegistryInvalidData);
            } else {
                QCOMPARE(Registry {object}, registry);
            }
        }

        void tst_Registry::urlToQUrl()
        {
            const Registry& registry = helperGetRegistry();
            QCOMPARE(registry.urlToQUrl(), QUrl("link1"));
        }

        void tst_Registry::infoUrlToQUrl()
        {
            const Registry& registry = helperGetRegistry();
            QCOMPARE(registry.infoUrlToQUrl(), QUrl("link11"));
        }

    }

}

QTEST_MAIN(modules::tests::tst_Registry)

#include "tst_registry.moc"
