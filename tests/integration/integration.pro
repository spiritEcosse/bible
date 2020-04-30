TEMPLATE = app
PROJECT_DIR = $$PWD/../../Application/src/

QT +=   sql \
        testlib \
        network
QT -=   gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

PROJECT_SOURCES = \
    $$PROJECT_DIR/DownloadManager.cpp \
    $$PROJECT_DIR/TextProgressBar.cpp \
    $$PROJECT_DIR/dbmanager.cpp \
    $$PROJECT_DIR/ModulesModel.cpp \
    $$PROJECT_DIR/ModulesGroupModel.cpp

PROJECT_HEADERS = \
    $$PROJECT_DIR/DownloadManager.h \
    $$PROJECT_DIR/TextProgressBar.h \
    $$PROJECT_DIR/dbmanager.h \
    $$PROJECT_DIR/ModulesModel.h \
    $$PROJECT_DIR/ModulesGroupModel.h

HEADERS += $${PROJECT_HEADERS}
SOURCES += $${PROJECT_SOURCES} \
    tst_downloadmodules.cpp \
    main.cpp

INCLUDEPATH += 3rdparty/quazip/include
LIBS += -L"3rdparty/quazip/lib" -lquazip

INCLUDEPATH = $$PROJECT_DIR
DEPENDPATH = $$PROJECT_DIR
