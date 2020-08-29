void tst_RegistryInfo::download_data()
{
    QJsonObject jsonObject;
    jsonObject.insert(QString("version"), 1234);
    QJsonDocument document(jsonObject);

    file.open(QFile::WriteOnly);
    file.write(document.toJson());
    file.close();
}

void tst_RegistryInfo::download()
{
    RegistryInfo registry;
//    QSignalSpy spy(&registry.manager, &DownloadManager::successfully);
    QSignalSpy spyLast(&registry, &RegistryInfo::newRegistryAvailable);
    QString registryUrl = QString("%1%2").arg(strUrl, file.fileName());
    QByteArray registryQ(registryUrl.toLocal8Bit());

    registry.download(registryQ.toBase64());
    QVERIFY(spyLast.wait());
//    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyLast.count(), 1);
    QList<QVariant> arguments = spyLast.takeFirst();
    QVERIFY(arguments.at(0).type() == QVariant::Bool);
    QCOMPARE(arguments.at(0).toBool(), true);
}
