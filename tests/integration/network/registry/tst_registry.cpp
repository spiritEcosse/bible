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
    const QString pathFiles { "../../../files" };
    const QString dirDownload = "download";
    const QString strUrl = "http://0.0.0.0:2443/files/";
    QFile fileRegistry { "registry.json" };
    const QFile fileRegistryArchive { "registry.zip" };
    const int fileRegistryItems = 5;
    QDir dir;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void download_data();
    void download();
    void removeRegistry_data();
    void removeRegistry();
};

void tst_Registry::cleanupTestCase()
{
    dir.rmdir(dirDownload);
}

void tst_Registry::initTestCase()
{
    // Will be called before the first test function is executed.
    dir.mkdir(pathFiles);
    dir.setCurrent(pathFiles);
    dir.mkdir(dirDownload);
}

tst_Registry::tst_Registry()
{

}

tst_Registry::~tst_Registry()
{

}

void tst_Registry::download_data()
{
    if (!fileRegistryArchive.exists()) {
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

        QVERIFY(JlCompress::compressFile(fileRegistryArchive.fileName(), fileRegistry.fileName()));
    }
}

void tst_Registry::download()
{
    Registry registry;
    QSignalSpy spy(&registry.manager, &DownloadManager::successfully);
    QSignalSpy spyLast(&registry, &Registry::decompressSuccess);

    QString registryUrl = QString("%1%2").arg(strUrl, fileRegistryArchive.fileName());
    QByteArray registryQ(registryUrl.toLocal8Bit());
    registry.download(registryQ.toBase64());
    QVERIFY(spyLast.wait());
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyLast.count(), 1);
}

void tst_Registry::removeRegistry_data()
{
    QFile fileRegistryArchiveD ( QString("%1/%2").arg(dirDownload, fileRegistryArchive.fileName()) );
    fileRegistryArchiveD.open(QFile::WriteOnly);
    fileRegistryArchiveD.close();

    QFile fileRegistryD ( QString("%1/%2").arg(dirDownload, fileRegistry.fileName()) );
    fileRegistryD.open(QFile::WriteOnly);
    fileRegistryD.close();
}

void tst_Registry::removeRegistry()
{
    Registry registry;
    QSignalSpy spy(&registry, &Registry::removeRegistrySuccess);
    registry.registryArchive.setFileName(QString("%1/%2").arg(dirDownload, fileRegistryArchive.fileName()));
    registry.file.setFileName(QString("%1/%2").arg(dirDownload, fileRegistry.fileName()));
    registry.removeRegistry();

    QCOMPARE(spy.count(), 1);
    QVERIFY(!registry.registryArchive.exists());
    QVERIFY(!registry.file.exists());
}

QTEST_MAIN(tst_Registry)

#include "tst_registry.moc"
