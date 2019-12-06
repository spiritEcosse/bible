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
    qt/QtSql/QSqlDatabase \
    qt/QtSql/qsqldatabase.h \
    qt/QtSql/QSqlError \
    qt/QtSql/qsqlerror.h \
    qt/QtSql/QSqlQuery \
    qt/QtSql/qsqlquery.h \
    qt/QtSql/QSqlQueryModel \
    qt/QtSql/qsqlquerymodel.h \
    qt/QtSql/QSqlRecord \
    qt/QtSql/qsqlrecord.h \
    qt/QtSql/QSqlTableModel \
    qt/QtSql/qsqltablemodel.h \
    qt/QtNetwork/qnetworkrequest.h \
    qt/QtCore/QQueue \
    qt/QtCore/QtCore \
    qt/QtCore/qjsonarray.h \
    qt/QtNetwork/QNetworkAccessManager \
    qt/QtNetwork/qnetworkaccessmanager.h \
    qt/QtCore/qiodevice.h \
    qt/QtCore/QIODevice \
    qt/QtCore/qdatetime.h \
    qt/QtCore/QTime \
    qt/QtCore/qqueue.h \
    qt/QtCore/qurl.h \
    qt/QtCore/Qurl \
    qt/QtCore/QFile \
    qt/QtCore/qfile.h \
    qt/QtCore/QVariant \
    qt/QtCore/qvariant.h \
    qt/QtCore/QFileInfo \
    qt/QtCore/qfileinfo.h \
    qt/QtNetwork/QNetworkReply \
    qt/QtNetwork/qnetworkreply.h \
    qt/QtNetwork/qsslerror.h \
    qt/QtNetwork/QSslError \
    qt/QtCore/QStringList \
    qt/QtCore/qstringlist.h \
    mock_modulesgroupmodel.h \
    mock_downloadmanager.h \
    mock_qfile.h \
    mock_qfileinfo.h \
    mock_qjsonarray.h \
    mock_qnetworkaccessmanager.h \
    mock_qnetworkreply.h \
    mock_qstringlist.h \
    mock_qtime.h \
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
    mock_textprogressbar.h


SOURCES += $$PROJECT_SOURCES \
    modulesgroupmodel-test.cpp \
    modulesmodel-test.cpp \
    main.cpp \
    downloadmanager-test.cpp \
    textprogressbar-test.cpp


QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

INCLUDEPATH += $$PWD/qt/QtNetwork + $$PWD/qt/QtSql + $$PWD/qt/QtCore
DEPENDPATH += $$PWD/qt/QtNetwork + $$PWD/qt/QtSql + $$PWD/qt/QtCore

LIBS += -lgmock -lgtest -lpthread
