TEMPLATE = app
PROJECT_DIR = $$PWD/../

QT -= gui

PROJECT_SOURCES = \
    $$PROJECT_DIR/src/DownloadManager.cpp \
    $$PROJECT_DIR/src/TextProgressBar.cpp \
    $$PROJECT_DIR/src/dbmanager.cpp \
    $$PROJECT_DIR/src/ModulesModel.cpp \
    $$PROJECT_DIR/src/ModulesGroupModel.cpp \
    $$PROJECT_DIR/src/booksmodel.cpp

PROJECT_HEADERS = \
    $$PROJECT_DIR/src/DownloadManager.h \
    $$PROJECT_DIR/src/TextProgressBar.h \
    $$PROJECT_DIR/src/ModulesModel.h \
    $$PROJECT_DIR/src/ModulesGroupModel.h \
    $$PROJECT_DIR/src/booksmodel.h

HEADERS += $$PROJECT_HEADERS \
    QtSql/QSqlDatabase \
    QtSql/qsqldatabase.h \
    QtSql/QSqlError \
    QtSql/qsqlerror.h \
    QtSql/QSqlQuery \
    QtSql/qsqlquery.h \
    QtSql/QSqlRecord \
    QtSql/QSqlQueryModel \
    QtSql/qsqlquerymodel.h \
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
    mock_qfile.h \
    mock_qfileinfo.h \
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
    mock_qqueue.h \
    mock_qurl.h \
    mock_qvariant.h \
    mock_textprogressbar.h \
    mock_modulesmodel.h \
    mock_modulesgroupmodel.h \
    mock_downloadmanager.h \
    gtest_global.h

SOURCES += $$PROJECT_SOURCES \
    modulesgroupmodel-test.cpp \
    modulesmodel-test.cpp \
    main.cpp \
    downloadmanager-test.cpp \
    textprogressbar-test.cpp

INCLUDEPATH += $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore
DEPENDPATH += $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore

LIBS += -lgmock -lgtest -lpthread
