QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase network
CONFIG -= app_bundle

TEMPLATE = app

include($$PWD/../../../../conf.pri)
SOURCES +=  tst_modulesgroupsfiller.cpp
