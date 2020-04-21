TEMPLATE = app
PROJECT_DIR = $$PWD/../../

QT +=   sql \
        testlib \
        network
QT -=   gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

PROJECT_SOURCES = \
    $$PROJECT_DIR/src/DownloadManager.cpp \
    $$PROJECT_DIR/src/TextProgressBar.cpp \
    $$PROJECT_DIR/src/dbmanager.cpp \
    $$PROJECT_DIR/src/ModulesModel.cpp \
    $$PROJECT_DIR/src/ModulesGroupModel.cpp

PROJECT_HEADERS = \
    $$PROJECT_DIR/src/DownloadManager.h \
    $$PROJECT_DIR/src/TextProgressBar.h \
    $$PROJECT_DIR/src/dbmanager.h \
    $$PROJECT_DIR/src/ModulesModel.h \
    $$PROJECT_DIR/src/ModulesGroupModel.h

HEADERS += $${PROJECT_HEADERS}
SOURCES += $${PROJECT_SOURCES} \
    tst_downloadmodules.cpp \
    main.cpp
