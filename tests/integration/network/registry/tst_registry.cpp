#include <QtTest>
#include "registry.h"

class tst_Registry : public QObject
{
    Q_OBJECT

public:
    tst_Registry();
    ~tst_Registry();

private slots:
    void download();
};

tst_Registry::tst_Registry()
{

}

tst_Registry::~tst_Registry()
{

}

void tst_Registry::download()
{
    QSignalSpy spy(&modulesGroupModel, &ModulesGroupModel::decompressSuccess);
    QSignalSpy spy1(&modulesGroupModel, &ModulesGroupModel::updateTableSuccess);
    QSignalSpy spy2(&modulesGroupModel, &ModulesGroupModel::removeRegistryFileSuccess);
    QSignalSpy spy3(&modulesGroupModel, &ModulesGroupModel::removeOldRowsSuccess);
    QString registryUrl = QString("%1%2").arg(strUrl, fileNameRegistryZip);
    QByteArray registry(registryUrl.toLocal8Bit());
    modulesGroupModel.registryBase64 = registry.toBase64();
    modulesGroupModel.updateModules();

    QVERIFY(spy2.wait());
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 1);
    QCOMPARE(spy3.count(), 1);

    Registry registry;
    registry.download();

//    ModulesGroupModel modulesGroupModelNew;
////    QCOMPARE(modulesGroupModelNew.rowCount(), fileRegistryItems);
//    modulesGroupModelNew.setCountOldRows();
//    modulesGroupModelNew.removeOldRows();
}

QTEST_APPLESS_MAIN(tst_Registry)

#include "tst_registry.moc"
