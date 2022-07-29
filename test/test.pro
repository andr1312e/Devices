QT += testlib
QT -= gui

include(../program/build.pri)

TEMPLATE = app

SOURCES +=  \
    messagestest.cpp

HEADERS += \
    defines.h \
    messagestest.h
