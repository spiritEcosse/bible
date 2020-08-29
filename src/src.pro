TEMPLATE = subdirs

# The name of your application
#TARGET = bible

#CONFIG += sailfishapp

#QT += core \
#    sql \
#    widgets \
#    network \
#    quick

#DISTFILES += qml/bible.qml \
#    qml/cover/CoverPage.qml \
#    qml/pages/FlippingPageHeader.qml \
#    qml/pages/History.qml \
#    rpm/bible.changes.in \
#    rpm/bible.changes.run.in \
#    rpm/bible.spec \
#    rpm/bible.yaml \
#    translations/*.ts \
#    bible.desktop \
#    qml/pages/PageEdgeTransition.qml \
#    qml/pages/CoverLoader.js \
#    qml/pages/private/CoverWindow.qml \
#    qml/pages/private/DefaultCover.qml \
#    qml/pages/private/LayoutGrid.qml \
#    qml/pages/private/PageStackGlassIndicator.qml \
#    qml/pages/private/PageStackIndicator.qml \
#    qml/pages/private/ReturnToHomeHintCounter.qml \
#    qml/pages/Pages.qml \
#    qml/pages/Modules.qml \
#    qml/pages/ModuleInformation.qml \
#    qml/pages/ExpandingSectionGroupPatch.qml \
#    qml/pages/BooksPage.qml \
#    qml/pages/VersesListView.qml \
#    qml/pages/ExpandingSectionBooks.qml \
#    qml/pages/ExpandingSectionPatch.qml \
#    qml/pages/CommentsPage.qml

## install
##target.path = $$[QT_INSTALL_EXAMPLES]/network/download
##INSTALLS += target

#SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

## to disable building translations every time, comment out the
## following CONFIG line
#CONFIG += sailfishapp_i18n
##    sailfishapp_i18n_idbased \

## German translation is enabled as an example. If you aren't
## planning to localize your app, remember to comment out the
## following TRANSLATIONS line. And also do not forget to
## modify the localized app name in the the .desktop file.
#TRANSLATIONS += translations/bible-ru.ts

##/usr/lib/qt5/qml/Sailfish/Silica/

#SOURCES += \
#    bible.cpp
#    dbmanager.cpp \
#    booksmodel.cpp \
#    modulesgroups.cpp \
#    modulesgroupsform.cpp \
#    modulesgroupsmodel.cpp \
#    modulesgroupsreader.cpp \
#    modulesmodel.cpp \
#    versesmodel.cpp \
#    userdb.cpp \
#    host.cpp \
#    registry.cpp \
#    qsortfiltersqlquerymodel.cpp \
#    historymodel.cpp \
#    commentsmodel.cpp \
#    LocaleDesc.cpp \
#    DownloadManager.cpp \
#    TextProgressBar.cpp

#HEADERS += \
#    dbmanager.h \
#    booksmodel.h \
#    modulesgroups.h \
#    modulesgroupsform.h \
#    modulesgroupsmodel.h \
#    modulesgroupsreader.h \
#    modulesmodel.h \
#    versesmodel.h \
#    userdb.h \
#    host.h \
#    registry.h \
#    global.h \
#    qsortfiltersqlquerymodel.h \
#    historymodel.h \
#    commentsmodel.h \
#    LocaleDesc.h \
#    DownloadManager.h \
#    TextProgressBar.h

#INCLUDEPATH += $$PWD/../../shared/include

#LIBS += -L$$PWD/../../shared/lib -lDBManager

SUBDIRS += \
    core \
    modules

CONFIG += ordered
