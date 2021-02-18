#include <QtQuick>
#include <sailfishapp.h>

//#include "booksmodel.h"
//#include "historymodel.h"
//#include "commentsmodel.h"
#include "modelgroupmodules.h"

void createAppDir() {
    const QDir writeDirApp = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!writeDirApp.mkpath("."))
        qFatal("Failed to create writable directory at %s", qPrintable(writeDirApp.absolutePath()));

    writeDirApp.setCurrent(writeDirApp.path());
    writeDirApp.mkdir("download");
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

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    QCoreApplication::setOrganizationName("spirit");
    QCoreApplication::setApplicationName("bible");

    createAppDir();

    modules::ModelGroupModules::registerMe();
//    qmlRegisterType<BooksModel>("bible.BooksModel", 1, 0, "BooksModel");
//    qmlRegisterType<CommentsModel>("bible.CommentsModel", 1, 0, "CommentsModel");
//    qmlRegisterType<ModulesModel>("bible.ModulesModel", 1, 0, "ModulesModel");
//    qmlRegisterType<HistoryModel>("bible.HistoryModel", 1, 0, "HistoryModel");

//    connectToDatabase();
//    ModuleProxyModel moduleProxyModel;
//    ModuleModel ModuleModel;
//    moduleProxyModel.setSourceModel(&ModuleModel);
//    moduleProxyModel.setSortRole(ModuleModel::SectionRole);
//    moduleProxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
//    moduleProxyModel.setSortCaseSensitivity(Qt::CaseInsensitive);
//    moduleProxyModel.setDynamicSortFilter(true);
//    moduleProxyModel.sort(0, Qt::AscendingOrder);
//    ctxt->setContextProperty("moduleProxyModel", &moduleProxyModel);

    view->setSource(SailfishApp::pathToMainQml());
    view->show();
    return app->exec();
}
