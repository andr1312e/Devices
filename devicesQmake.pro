QT += serialport network
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG += no_lflags_merge rtti_off warn_on c++latest strict_c++ precompile_header utf8_source
CONFIG -= cmdline qml_debug no_include_pwd

DEFINES +=                 \
  QT_DISABLE_DEPRECATED_BEFORE=0x050B00 \# отключить API, устаревший в 5.11 или более ранних версиях
  QT_USE_FAST_CONCATENATION\
  QT_DEPRECATED_WARNINGS   \
  QT_USE_FAST_OPERATOR_PLUS\
  DQT_NO_URL_CAST_FROM_STRING\
  QT_USE_STRING_BUILDER \

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
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
    program.cpp

HEADERS += \
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
    program.h

DISTFILES +=

RESOURCES += \
    qrc.qrc
