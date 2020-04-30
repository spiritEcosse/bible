TEMPLATE = lib
QT -= qui
QT += sql core
CONFIG += c++11

DEFINES += BUILD_TESTS

DESTDIR = $$PWD/../shared/lib

INCLUDEPATH = $$PWD/include

HEADERS += \
    include/Executor.h \
    include/Processor.h \
    include/Selector.h \
    include/dbmapper.h \
    include/dbtypes.h \
    include/ConnectionManager.h

SOURCES += \
    src/ConnectionManager.cpp \
    src/Executor.cpp \
    src/Processor.cpp \
    src/Selector.cpp
