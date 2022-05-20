//#include <QtQuick>
#ifdef SAILFISH
#include <sailfishapp.h>
#endif

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QQuickStyle>
#include <QIcon>

#include "booksmodel.h"
#include "historymodel.h"
#include "commentsmodel.h"
#include "modelgroupmodules.h"
#include "quickdownload.h"
#include "dbmanager.h"

void createAppDir() {
    const QDir writeDirApp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!writeDirApp.mkpath("."))
        qFatal("Failed to create writable directory at %s", qPrintable(writeDirApp.absolutePath()));

    writeDirApp.setCurrent(writeDirApp.path());
    writeDirApp.mkdir("download");
    writeDirApp.mkdir("modules");
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/bible.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    QCoreApplication::setOrganizationName("spirit");
    QCoreApplication::setApplicationName("bible");

    createAppDir();

    connectToDatabase();
    modules::ModelGroupModules::registerMe();
    qmlRegisterType<netmanager::QuickDownload>("bible.QuickDownload", 1, 0, "QuickDownload");
    modules::ModelModule::registerMe();
    qmlRegisterType<BooksModel>("bible.BooksModel", 1, 0, "BooksModel");
    qmlRegisterType<CommentsModel>("bible.CommentsModel", 1, 0, "CommentsModel");
//    qmlRegisterType<ModulesModel>("bible.ModulesModel", 1, 0, "ModulesModel");
    qmlRegisterType<HistoryModel>("bible.HistoryModel", 1, 0, "HistoryModel");

//    ModuleProxyModel moduleProxyModel;
//    ModuleModel ModuleModel;
//    moduleProxyModel.setSourceModel(&ModuleModel);
//    moduleProxyModel.setSortRole(ModuleModel::SectionRole);
//    moduleProxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
//    moduleProxyModel.setSortCaseSensitivity(Qt::CaseInsensitive);
//    moduleProxyModel.setDynamicSortFilter(true);
//    moduleProxyModel.sort(0, Qt::AscendingOrder);
//    ctxt->setContextProperty("moduleProxyModel", &moduleProxyModel);

#ifdef SAILFISH
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->setSource(SailfishApp::pathToMainQml());
    view->show();
    return app->exec();
#else
    QGuiApplication app(argc, argv);

    QIcon::setThemeName("bible");

    QSettings settings;

    qDebug() << qEnvironmentVariableIsEmpty("QML_FORCE_DISK_CACHE");

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle("Material");

    // If this is the first time we're running the application,
    // we need to set a style in the settings so that the QML
    // can find it in the list of built-in styles.
    const QString styleInSettings = settings.value("style").toString();
    if (styleInSettings.isEmpty())
        settings.setValue(QLatin1String("style"), QQuickStyle::name());

//    qDebug() << settings.value("style").toString() << QQuickStyle::name();

    QQmlApplicationEngine engine;

    QStringList builtInStyles = { QLatin1String("Basic"), QLatin1String("Fusion"),
        QLatin1String("Imagine"), QLatin1String("Material"), QLatin1String("Universal") };
#if defined(Q_OS_MACOS)
    builtInStyles << QLatin1String("macOS");
#elif defined(Q_OS_WINDOWS)
    builtInStyles << QLatin1String("Windows");
#endif

    engine.setInitialProperties({{ "builtInStyles", builtInStyles }});
    engine.load(QUrl("qrc:/bible.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
#endif
}
