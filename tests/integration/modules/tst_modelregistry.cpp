#include <QtTest>
#include "modelregistry.h"

namespace TestModelRegistry {

class tst_ModelRegistry : public QObject {
    Q_OBJECT

private:
    QJsonDocument helperGetDocument();

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
    array << QJsonObject {{"url", "link2"}, {"priority", 2}, {"info_url", "link22"}};
    array << QJsonObject {{"url", "link3"}, {"priority", 3}, {"info_url", "link33"}};

    return QJsonDocument {
        QJsonObject {
            { "registries",  array }
        }
    };
}

tst_ModelRegistry::tst_ModelRegistry()
{

}

tst_ModelRegistry::~tst_ModelRegistry()
{

}

void tst_ModelRegistry::update()
{
    ModelRegistry modelRegistry;
    QSignalSpy spyLast(&modelRegistry, &ModelRegistry::updateSuccess);

    modelRegistry.update(helperGetDocument());
    QCOMPARE(spyLast.count(), 1);
}

}

QTEST_MAIN(TestModelRegistry::tst_ModelRegistry)

#include "tst_modelregistry.moc"
