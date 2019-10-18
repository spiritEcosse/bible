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
    QtSql/QSqlDatabase \
    QtSql/qsqldatabase.h \
    QtSql/QSqlError \
    QtSql/qsqlerror.h \
    QtSql/QSqlQuery \
    QtSql/qsqlquery.h \
    QtSql/QSqlRecord \
    QtSql/qsqlrecord.h \
    QtSql/QSqlTableModel \
    QtSql/qsqltablemodel.h \
    QtNetwork/qnetworkrequest.h \
    QtCore/QQueue \
    QtCore/QtCore \
    QtCore/qqueue.h \
    QtCore/qurl.h \
    QtCore/Qurl \
    QtCore/qstring.h \
    QtCore/QString \
    mock_modulesgroupmodel.h \
    mock_downloadmanager.h \
    mock_qstring.h \
    mock_qtimer.h \
    mock_qnetworkrequest.h \
    mock_qsqlquery.h \
    mock_qsqldatabase.h \
    mock_qsqlrecord.h \
    mock_qsqlerror.h \
    gtest_global.h \
    mock_qqueue.h \
    mock_qurl.h

SOURCES += $$PROJECT_SOURCES \
    modulesgroupmodel-test.cpp \
    modulesmodel-test.cpp \
    main.cpp \
    downloadmanager-test.cpp

INCLUDEPATH += $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore
DEPENDPATH += $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore

LIBS += -lquazip5 -lgmock -lgtest -lpthread
