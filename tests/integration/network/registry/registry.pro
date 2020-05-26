QT += testlib
QT -= gui
QT += network

CONFIG += qt console warn_on depend_includepath testcase network
CONFIG -= app_bundle

TEMPLATE = app
include($$PWD/../../../../conf.pri)
SOURCES +=  tst_registry.cpp
