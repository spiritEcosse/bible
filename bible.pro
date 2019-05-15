# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = bible

CONFIG += sailfishapp

QT += core \
    sql \
    widgets \
    testlib \
    network

TEMPLATE = app

DISTFILES += qml/bible.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FlippingPageHeader.qml \
    qml/pages/History.qml \
    rpm/bible.changes.in \
    rpm/bible.changes.run.in \
    rpm/bible.spec \
    rpm/bible.yaml \
    translations/*.ts \
    bible.desktop \
    qml/pages/PageEdgeTransition.qml \
    qml/pages/CoverLoader.js \
    qml/pages/private/CoverWindow.qml \
    qml/pages/private/DefaultCover.qml \
    qml/pages/private/LayoutGrid.qml \
    qml/pages/private/PageStackGlassIndicator.qml \
    qml/pages/private/PageStackIndicator.qml \
    qml/pages/private/ReturnToHomeHintCounter.qml \
    qml/pages/Pages.qml \
    qml/pages/Modules.qml \
    registry/registry.json \
    qml/pages/ModuleInformation.qml \
    qml/pages/ExpandingSectionGroupPatch.qml \
    qml/pages/BooksPage.qml \
    qml/pages/VersesListView.qml \
    qml/pages/ExpandingSectionBooks.qml \
    qml/pages/ExpandingSectionPatch.qml \
    qml/pages/CommentsPage.qml

db.files = db
db.path = /usr/share/$${TARGET}
INSTALLS += db

registry.files = registry
registry.path = /usr/share/$${TARGET}
INSTALLS += registry

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/network/download
#INSTALLS += target

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
#    sailfishapp_i18n_idbased \

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/bible-ru.ts

#/usr/lib/qt5/qml/Sailfish/Silica/

SOURCES += src/dbmanager.cpp \
    src/bible.cpp \
    src/booksmodel.cpp \
    src/versesmodel.cpp \
    src/userdb.cpp \
    src/host.cpp \
    src/registry.cpp \
    src/qsortfiltersqlquerymodel.cpp \
    src/historymodel.cpp \
    src/commentsmodel.cpp \
    src/LocaleDesc.cpp \
    src/ModulesModel.cpp \
    src/DownloadManager.cpp \
    src/TextProgressBar.cpp \
    src/ModulesGroupModel.cpp

HEADERS += \
    src/dbmanager.h \
    src/booksmodel.h \
    src/versesmodel.h \
    src/userdb.h \
    src/host.h \
    src/registry.h \
    src/global.h \
    src/qsortfiltersqlquerymodel.h \
    src/historymodel.h \
    src/commentsmodel.h \
    src/LocaleDesc.h \
    src/ModulesModel.h \
    src/DownloadManager.h \
    src/TextProgressBar.h \
    src/ModulesGroupModel.h

LIBS += -lquazip
