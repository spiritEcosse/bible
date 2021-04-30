#include <QtTest>
#include "host.h"

#include "entitybase.h"

Q_DECLARE_METATYPE(modules::Host)

namespace modules {

    namespace tests {

        class tst_Host : public ::tests::EntityBase<Host> {
            Q_OBJECT

        public:
            tst_Host();
            ~tst_Host();

        private:
            Host helperGetObject();

        private slots:
            void constructor_data();
            void constructor() override;
            void pathToQUrl();
        };

        tst_Host::tst_Host() {}

        tst_Host::~tst_Host() {}

        Host tst_Host::helperGetObject()
        {
            return Host ("alias", "bGluazEx", 100, 200);
        }

        void tst_Host::constructor_data()
        {
            QTest::addColumn<QJsonObject>("json_object");
            QTest::addColumn<Host>("object");
            QTest::addColumn<bool>("except");

            QJsonObject data {{"alias", "alias"}, {"priority", 100}, {"path", "link11"}, {"weight", 200}};
            QTest::newRow("valid data") << data << helperGetObject() << false;

            data = {
                {"path", "link11"}
            };
            QTest::newRow("default fields") << data << Host ("", "bGluazEx", 0, 0) << false;

            data = {{"priority", 1}, {"alias", "alias"}};
            QTest::newRow("invalid data: required m_path.") << data << Host {} << true;
        }

        void tst_Host::constructor()
        {
            ::tests::EntityBase<Host>::constructor();
        }

        void tst_Host::pathToQUrl()
        {
            const Host& host = helperGetObject();
            QCOMPARE(host.pathToQUrl(), QUrl("link11"));
        }
    }

}

QTEST_MAIN(modules::tests::tst_Host)

#include "tst_host.moc"
