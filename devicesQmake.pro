QT += network  serialport
QT -= gui
VERSION = 8
TARGET = devices_v_$$VERSION

CONFIG += c++11 console no_lflags_merge rtti_off warn_on c++latest strict_c++ precompile_header utf8_source
CONFIG -= app_bundle cmdline qml_debug no_include_pwd

DEFINES +=                 \
  QT_DISABLE_DEPRECATED_BEFORE=0x050B00 \# отключить API, устаревший в 5.11 или более ранних версиях
  QT_USE_FAST_CONCATENATION\
  QT_DEPRECATED_WARNINGS   \
  QT_USE_FAST_OPERATOR_PLUS\
  DQT_NO_URL_CAST_FROM_STRING\
  QT_USE_STRING_BUILDER \

SOURCES += \
        main.cpp \
    messageRepositories/moxamessagerepository.cpp \
    sockets/geosocket.cpp \
    sockets/meteosocket.cpp \
    sockets/rarmsocket.cpp \
    sockets/ustirovsocket.cpp \
    messageRepositories/meteomessagerepository.cpp \
    messageRepositories/ustrirovmessagerepository.cpp \
    messageCreatrors/meteomessagegetter.cpp \
    messageCreatrors/statemessagegetter.cpp \
    messageCreatrors/statemessagesender.cpp \
    mediators/meteomediator.cpp \
    mediators/moxamediator.cpp \
    mediators/ustirovmediator.cpp \
    singleinstancemaker.cpp \
    devices.cpp \
    mediators/consolemediator.cpp

HEADERS += \
    messageRepositories/moxamessagerepository.h \
    sockets/geosocket.h \
    sockets/meteosocket.h \
    sockets/rarmsocket.h \
    sockets/ustirovsocket.h \
    messageRepositories/meteomessagerepository.h \
    messageRepositories/ustrirovmessagerepository.h \
    messageCreatrors/constants.h \
    messageCreatrors/meteomessagegetter.h \
    messageCreatrors/statemessagegetter.h \
    messageCreatrors/statemessagesender.h \
    mediators/meteomediator.h \
    mediators/moxamediator.h \
    mediators/ustirovmediator.h \
    datamessage.h \
    singleinstancemaker.h \
    devices.h \
    mediators/consolemediator.h \
    mediators/logger.h \
    enviroment.h

