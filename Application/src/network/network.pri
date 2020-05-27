INCLUDEPATH += $$PWD

HOME = $$PWD/../../../
INCLUDEPATH += $$HOME/3rdparty/quazip
LIBS += -L"$$HOME/3rdparty/quazip/quazip" -lquazip

SOURCES += \
    $$PWD/registry.cpp \
    $$PWD/textprogressbar.cpp \
    $$PWD/downloadmanager.cpp

HEADERS += \
    $$PWD/registry.h \
    $$PWD/textprogressbar.h \
    $$PWD/downloadmanager.h
