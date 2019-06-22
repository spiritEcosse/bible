QT += gui testlib sql network
CONFIG += qt warn_on depend_includepath testcase

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

HEADERS += $$PROJECT_HEADERS \
    mock_modulesmodel.h \
    iqsqldatabase.h \
    iqsqlquery.h \
    mock_iqsqldatabase.h

SOURCES += $$PROJECT_SOURCES \
    modulesgroupmodel-test.cpp \
    modulesmodel-test.cpp \
    main.cpp

LIBS += -lquazip5 -lgtest -lgmock
