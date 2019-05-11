QT += gui testlib sql network
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app
PROJECT_DIR = $$PWD/../

PROJECT_SOURCES = \
    $$PROJECT_DIR/src/DownloadManager.cpp \
    $$PROJECT_DIR/src/TextProgressBar.cpp \
    $$PROJECT_DIR/src/dbmanager.cpp \
    $$PROJECT_DIR/src/ModulesModel.cpp

PROJECT_HEADERS = \
    $$PROJECT_DIR/src/DownloadManager.h \
    $$PROJECT_DIR/src/TextProgressBar.h \
    $$PROJECT_DIR/src/ModulesModel.h

HEADERS += $$PROJECT_HEADERS

SOURCES += $$PROJECT_SOURCES \
    tst_tests.cpp

unix {
    LIBS += -L$$(QUAZIP_CODE) -lz
}

win32 {
    LIBS += -L$$(ZLIB_CODE) -lzdll
}

INCLUDEPATH += $$(QUAZIP_CODE)
HEADERS += $$(QUAZIP_CODE)/*.h
SOURCES += $$(QUAZIP_CODE)/*.cpp
SOURCES += $$(QUAZIP_CODE)/*.c
