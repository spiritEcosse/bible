#include <QtTest>
#include "registry.h"
#include <quazip/JlCompress.h>

class tst_Registry : public QObject
{
    Q_OBJECT

public:
    tst_Registry();
    ~tst_Registry();

private:
    const QString dirNameDownload = "download";
    QString pathFiles { "../../../../files" };
    const QString strUrl = "http://0.0.0.0:2443/files/";
//    QString strUrlTest = QString("%1test").arg(strUrl);
//    const QString fileNameRegistryInfo = "registry_info.json";
    QFile fileRegistry { "registry.json" };
    QFile fileRegistryZip { QString("%1.zip").arg(fileRegistry.fileName()) };
    const int fileRegistryItems = 5;
//    QString tableNameModules = "modules";
//    QString tableNameGroupModules = "modules_group";
//    QFile fileRegistryInfo;
    QDir dir;
//    QSettings settings;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void download_data();
    void download();
};

void tst_Registry::cleanupTestCase()
{
    dir.rmdir(dirNameDownload);
}

void tst_Registry::initTestCase()
{
    // Will be called before the first test function is executed.
    dir.mkdir(pathFiles);
    dir.setCurrent(pathFiles);
    dir.mkdir(dirNameDownload);

    QFile test("test");
    test.open(QFile::ReadOnly);
    test.close();
}

tst_Registry::tst_Registry()
{

}

tst_Registry::~tst_Registry()
{

}

void tst_Registry::download_data()
{
    if (!fileRegistryZip.exists()) {
        QJsonArray jsonArray;

        auto downloads = QJsonObject(
        {
                        qMakePair(QString("fil"), QJsonValue("name")),
                        qMakePair(QString("des"), QJsonValue("description")),
                        qMakePair(QString("abr"), QJsonValue("abbreviation")),
                        qMakePair(QString("inf"), QJsonValue("information")),
                        qMakePair(QString("lng"), QJsonValue("language")),
                        qMakePair(QString("aln"), QJsonValue("language_show")),
                        qMakePair(QString("upd"), QJsonValue("update")),
                        qMakePair(QString("url"), QJsonValue("urls")),
                        qMakePair(QString("cmt"), QJsonValue("comment")),
                        qMakePair(QString("siz"), QJsonValue("1")),
                        qMakePair(QString("reg"), QJsonValue("region")),
                        qMakePair(QString("def"), QJsonValue().toBool(true)),
                        qMakePair(QString("hid"), QJsonValue().toBool()),
                        qMakePair(QString("lic"), QJsonValue("copyright")),
        });

        for (int i = 0; i < fileRegistryItems; i++) {
            jsonArray << QJsonValue(downloads);
        }

        QJsonObject jsonObject;
        jsonObject.insert(QString("downloads"), QJsonValue(jsonArray));
        QJsonDocument document(jsonObject);

        fileRegistry.open(QFile::WriteOnly);
        fileRegistry.write(document.toJson());
        fileRegistry.close();

        QVERIFY(JlCompress::compressFile(fileRegistryZip.fileName(), fileRegistry.fileName()));
    }
}

void tst_Registry::download()
{
    Registry registry;
    QSignalSpy spy(&registry.manager, &DownloadManager::successfully);
    QSignalSpy spyLast(&registry, &Registry::decompressSuccess);

    QString registryUrl = QString("%1%2").arg(strUrl, fileRegistryZip.fileName());
    QByteArray registryQ(registryUrl.toLocal8Bit());
    registry.download(registryQ.toBase64());
    QVERIFY(spyLast.wait());
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyLast.count(), 1);
}

QTEST_MAIN(tst_Registry)

#include "tst_registry.moc"
