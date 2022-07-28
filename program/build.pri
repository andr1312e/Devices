QT += network
CONFIG += serialPortUse

serialPortUse {
    QT +=serialport
    message("Serial configuration")
} else {
    message("Unknown configuration")
}
QT -= gui

SOURCES += \
    $$PWD/messageCreatrors/ustirovmessagegegetter.cpp \
    $$PWD/messageCreatrors/ustirovmessagesender.cpp \
    $$PWD/messageRepositories/geomessagerepository.cpp \
    $$PWD/messageRepositories/moxamessagerepository.cpp \
    $$PWD/sockets/meteosocket.cpp \
    $$PWD/sockets/rarmsocket.cpp \
    $$PWD/sockets/ustirovsocket.cpp \
    $$PWD/messageRepositories/meteomessagerepository.cpp \
    $$PWD/messageRepositories/ustrirovmessagerepository.cpp \
    $$PWD/messageCreatrors/meteomessagegetter.cpp \
    $$PWD/mediators/meteomediator.cpp \
    $$PWD/mediators/moxamediator.cpp \
    $$PWD/mediators/ustirovmediator.cpp \
    $$PWD/singleinstancemaker.cpp \
    $$PWD/devices.cpp \
    $$PWD/mediators/consolemediator.cpp

HEADERS += \
    $$PWD/messageCreatrors/ustirovmessagegegetter.h \
    $$PWD/messageCreatrors/ustirovmessagesender.h \
    $$PWD/messageRepositories/geomessagerepository.h \
    $$PWD/messageRepositories/moxamessagerepository.h \
    $$PWD/sockets/meteosocket.h \
    $$PWD/sockets/rarmsocket.h \
    $$PWD/sockets/ustirovsocket.h \
    $$PWD/messageRepositories/meteomessagerepository.h \
    $$PWD/messageRepositories/ustrirovmessagerepository.h \
    $$PWD/messageCreatrors/constants.h \
    $$PWD/messageCreatrors/meteomessagegetter.h \
    $$PWD/mediators/meteomediator.h \
    $$PWD/mediators/moxamediator.h \
    $$PWD/mediators/ustirovmediator.h \
    $$PWD/datamessage.h \
    $$PWD/singleinstancemaker.h \
    $$PWD/devices.h \
    $$PWD/mediators/consolemediator.h \
    $$PWD/mediators/logger.h \
    $$PWD/enviroment.h
