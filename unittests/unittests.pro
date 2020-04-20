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
    $$PROJECT_DIR/src/dbmanager.h \
    $$PROJECT_DIR/src/ModulesModel.h \
    $$PROJECT_DIR/src/ModulesGroupModel.h

HEADERS += $$PROJECT_HEADERS \
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
    QtNetwork/QNetworkRequest \
    QtNetwork/qnetworkrequest.h \
    QtCore/QQueue \
    QtCore/qqueue.h \
    QtCore/QtCore \
    QtCore/QJsonArray \
    QtCore/qjsonarray.h \
    QtNetwork/QNetworkAccessManager \
    QtNetwork/qnetworkaccessmanager.h \
    QtCore/QIODevice \
    QtCore/qiodevice.h \
    QtCore/QDateTime \
    QtCore/qdatetime.h \
    QtCore/QTime \
    QtCore/QUrl \
    QtCore/qurl.h \
    QtCore/QFile \
    QtCore/qfile.h \
    QtCore/QVariant \
    QtCore/qvariant.h \
    QtCore/QFileInfo \
    QtCore/qfileinfo.h \
    QtNetwork/QNetworkReply \
    QtNetwork/qnetworkreply.h \
    QtNetwork/QSslError \
    QtNetwork/qsslerror.h \
    QtCore/QStringList \
    QtCore/qstringlist.h \
    QtCore/qlist.h \
    QtCore/QString \
    QtCore/qstring.h \
    QtCore/QTextStream \
    QtCore/qtextstream.h \
    mock_qbytearray.h \
    mock_qjsondocument.h \
    QtCore/QJsonObject \
    mock_qjsonobject.h \
    mock_qjsonvalue.h \
    mock_qlist.h \
    mock_qsettings.h \
    mock_qstring.h \
    mock_modulesmodel.h \
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
    mock_textprogressbar.h \
    QtCore/qjsondocument.h \
    QtCore/qsettings.h \
    QtCore/qbytearray.h


QMAKE_POST_LINK += $$PWD/scripts/post_unit_tests.sh

SOURCES += $$PROJECT_SOURCES \
    modulesgroupmodel-test.cpp \
    modulesmodel-test.cpp \
    main.cpp \
    downloadmanager-test.cpp \
    textprogressbar-test.cpp


QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

INCLUDEPATH = $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore
DEPENDPATH = $$PWD/QtNetwork + $$PWD/QtSql + $$PWD/QtCore

LIBS += -lgmock -lgtest -lpthread -lquazip5

DISTFILES += \
    QtCore/QJsonParseError \
    QtCore/QJsonDocument \
    QtCore/QJsonValue \
    QtCore/QList \
    QtCore/QSettings \
    QtCore/QByteArray

