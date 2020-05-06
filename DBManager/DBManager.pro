TEMPLATE = lib
QT -= qui
QT += sql core
CONFIG += c++11

DESTDIR = $$PWD/../shared/lib

INCLUDEPATH = $$PWD/include

HEADERS += \
    include/Executor.h \
    include/Processor.h \
    include/Selector.h \
    include/dbmapper.h \
    include/dbtypes.h \
    include/ConnectionManager.h \
    manipulator.h

SOURCES += \
    manipulator.cpp \
    src/ConnectionManager.cpp \
    src/Executor.cpp \
    src/Processor.cpp \
    src/Selector.cpp

headers.path = $$PWD/../shared/include
headers.files = include/dbtypes.h \
                include/Processor.h
INSTALLS += headers
