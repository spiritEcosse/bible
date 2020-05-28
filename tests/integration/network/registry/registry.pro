QT += testlib
QT -= gui
QT += network

CONFIG += qt console warn_on depend_includepath testcase network
CONFIG -= app_bundle

TEMPLATE = app
include($$PWD/../../../../conf.pri)
SOURCES +=  tst_registry.cpp

DISTFILES += \
    docker-compose.yml

copydata.commands = $(COPY_DIR) $$PWD/docker-compose.yml $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
