#include "tst_registry.h"

Q_DECLARE_METATYPE(modules::Registry)

namespace modules {

    namespace tests {

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
            EntityBase<Registry>::constructor();
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

#include "tst_registry.moc"
