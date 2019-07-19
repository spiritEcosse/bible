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

HEADERS += \
    mock_modulesmodel.h \
    iqsqldatabase.h \
    iqsqlquery.h \
    QtSql/qsqldatabase.h \
    mock_iqsqldatabase.h \
    mock_iqsqlquery.h \
    mock_modulesgroupmodel.h \
    mock_downloadmanager.h \
    mock_qtimer.h \
    mock_qnetworkrequest.h

SOURCES += \
    modulesgroupmodel-test.cpp \
    modulesmodel-test.cpp \
    main.cpp \
    downloadmanager-test.cpp

INCLUDEPATH += $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore
DEPENDPATH += $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore

LIBS += -lquazip5 -lgmock -lgtest -lpthread
