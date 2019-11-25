TEMPLATE = app
PROJECT_DIR = $$PWD/../

QT -= gui

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

HEADERS += $$PROJECT_HEADERS \
    mock_modulesmodel.h \
    QtSql/QSqlDatabase \
    QtSql/qsqldatabase.h \
    QtSql/QSqlError \
    QtSql/qsqlerror.h \
    QtSql/QSqlQuery \
    QtSql/qsqlquery.h \
    QtSql/QSqlQueryModel \
    QtSql/qsqlquerymodel.h \
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
    QtCore/QFile \
    QtCore/qfile.h \
    QtCore/QVariant \
    QtCore/qvariant.h \
    QtCore/QFileInfo \
    QtCore/qfileinfo.h \
    QtNetwork/QNetworkReply \
    QtNetwork/qnetworkreply.h \
    QtNetwork/qsslerror.h \
    QtNetwork/QSslError \
    mock_modulesgroupmodel.h \
    mock_downloadmanager.h \
    mock_qfile.h \
    mock_qfileinfo.h \
    mock_qjsonarray.h \
    mock_qnetworkaccessmanager.h \
    mock_qnetworkreply.h \
    QtCore/QStringList \
    QtCore/qstringlist.h \
    mock_qstringlist.h \
    mock_qtimer.h \
    mock_qnetworkrequest.h \
    mock_qsqlquery.h \
    mock_qsqldatabase.h \
    mock_qsqlrecord.h \
    mock_qsqlerror.h \
    gtest_global.h \
    mock_qqueue.h \
    mock_qurl.h \
    mock_qvariant.h \
    mock_textprogressbar.h \
    QtCore/qjsonarray.h \
    QtNetwork/QNetworkAccessManager \
    QtNetwork/qnetworkaccessmanager.h


SOURCES += $$PROJECT_SOURCES \
    modulesgroupmodel-test.cpp \
    modulesmodel-test.cpp \
    main.cpp \
    downloadmanager-test.cpp \
    textprogressbar-test.cpp

INCLUDEPATH += $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore
DEPENDPATH += $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore

LIBS += -lgmock -lgtest -lpthread
