QT += core network

INCLUDEPATH += $$PWD

HOME = $$PWD/../../../
INCLUDEPATH += $$HOME/3rdparty/quazip/
LIBS += -L"$$HOME/3rdparty/quazip/quazip" -lQt5Quazip

SOURCES += \
    $$PWD/modulesgroupsfiller.cpp \
    $$PWD/registryinfo.cpp \
    $$PWD/registry.cpp \
    $$PWD/textprogressbar.cpp \
    $$PWD/downloadmanager.cpp

HEADERS += \
    $$PWD/modulesgroupsfiller.h \
    $$PWD/registryinfo.h \
    $$PWD/registry.h \
    $$PWD/textprogressbar.h \
    $$PWD/downloadmanager.h

include($$PWD/../sql/models/models.pri)
include($$PWD/../core/core.pri)
