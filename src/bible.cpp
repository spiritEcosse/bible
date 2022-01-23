#include <QtQuick>
#ifdef SAILFISH
#include <sailfishapp.h>
#endif

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

    QQuickView view;
    view.setSource(QUrl("qrc:/bible.qml"));
    view.showMaximized();

    return app.exec();
#endif
}
