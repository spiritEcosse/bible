CONFIG += qt warn_on depend_includepath testcase console

TEMPLATE = app
PROJECT_DIR = $$PWD/../

PROJECT_SOURCES = \
    $$PROJECT_DIR/src/DownloadManager.cpp \
    $$PROJECT_DIR/src/TextProgressBar.cpp \
    $$PROJECT_DIR/src/dbmanager.cpp \
    $$PROJECT_DIR/src/ModulesModel.cpp \
    $$PROJECT_DIR/src/ModulesGroupModel.cpp

PROJECT_HEADERS = \
    $$PROJECT_DIR/src/DownloadManager.h \
    $$PROJECT_DIR/src/TextProgressBar.h \
    $$PROJECT_DIR/src/ModulesModel.h \
    $$PROJECT_DIR/src/ModulesGroupModel.h

GTEST_DIR = /home/igor/projects/googletest/googletest
GMOCK_DIR = /home/igor/projects/googletest/googlemock

HEADERS += $$PROJECT_HEADERS \
    mock_modulesmodel.h \
    iqsqldatabase.h \
    iqsqlquery.h \
    mock_iqsqldatabase.h \
    mock_iqsqlquery.h \
    mock_modulesgroupmodel.h \
    mock_downloadmanager.h \
    mock_iqtimer.h \
    iqtimer.h \
    qtnetwork.h

SOURCES += $$PROJECT_SOURCES \
    modulesgroupmodel-test.cpp \
    modulesmodel-test.cpp \
    main.cpp \
    downloadmanager-test.cpp

INCLUDEPATH += $$PWD/QtNetwork + $$PWD/QtSql
DEPENDPATH += $$PWD/QtNetwork + $$PWD/QtSql

LIBS += -lquazip5 -lgmock -lgtest -lpthread
