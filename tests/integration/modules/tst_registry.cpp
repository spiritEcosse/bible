#include <QtTest>
#include "registry.h"
#include "entitybase.h"

Q_DECLARE_METATYPE(modules::Registry)

namespace modules {

    namespace tests {

        class tst_Registry : public ::tests::EntityBase<Registry> {
            Q_OBJECT

        public:
            tst_Registry();
            ~tst_Registry();

        private:
            Registry helperGetObject();

        private slots:
            void constructor_data();
            void constructor() override;
            void urlToQUrl();
            void infoUrlToQUrl();
        };

        tst_Registry::tst_Registry() {}

        tst_Registry::~tst_Registry() {}

        Registry tst_Registry::helperGetObject()
        {
            return Registry ("bGluazE=", "bGluazEx", 1, true);
        }

        void tst_Registry::constructor_data()
        {
            QTest::addColumn<QJsonObject>("json_object");
            QTest::addColumn<Registry>("object");
            QTest::addColumn<bool>("except");

            QJsonObject data {{"url", "link1"}, {"priority", 1}, {"info_url", "link11"}, {"test", true}};
            QTest::newRow("valid data") << data << helperGetObject() << false;

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
            ::tests::EntityBase<Registry>::constructor();
        }

        void tst_Registry::urlToQUrl()
        {
            const Registry& registry = helperGetObject();
            QCOMPARE(registry.urlToQUrl(), QUrl("link1"));
        }

        void tst_Registry::infoUrlToQUrl()
        {
            const Registry& registry = helperGetObject();
            QCOMPARE(registry.infoUrlToQUrl(), QUrl("link11"));
        }

    }

}

QTEST_MAIN(modules::tests::tst_Registry)

#include "tst_registry.moc"
