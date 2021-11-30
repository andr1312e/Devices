cmake_minimum_required(VERSION 3.7.2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

macro(use_cxx11)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
  endif ()
endmacro(use_cxx11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_PREFIX_PATH "\\usr\\lib\\x86_64-linux-gnu\\cmake")

find_package(Qt5 COMPONENTS Core Network REQUIRED)

project(devices LANGUAGES CXX VERSION 0.0.1)
message(STATUS "DEVICES: PCB 687263.035 and meteo\nVersions:\n program : ${CMAKE_PROJECT_VERSION};\n cmake : ${CMAKE_VERSION};\n qt-core : ${Qt5Core_VERSION}; ")

set(
    SOURCE_LIST
    enum/uenum.h
    messageCreatrors/constants.h
    messageCreatrors/statemessagegetter.h
    messageCreatrors/statemessagegetter.cpp
    messageCreatrors/meteomessagegetter.h
    messageCreatrors/meteomessagegetter.cpp
    messageCreatrors/statemessagesender.h
    messageCreatrors/statemessagesender.cpp
    sockets/meteosocket.h
    sockets/meteosocket.cpp
    sockets/ustirovsocket.h
    sockets/ustirovsocket.cpp
    sockets/rarmsocket.h
    sockets/rarmsocket.cpp
    mediators/meteomediator.h
    mediators/meteomediator.cpp
    mediators/ustirovmediator.h
    mediators/ustirovmediator.cpp
    messageRepositories/ustrirovmessagerepository.h
    messageRepositories/ustrirovmessagerepository.cpp
    messageRepositories/meteomessagerepository.h
    messageRepositories/meteomessagerepository.cpp
    datamessage.h
    program.h
    program.cpp
    main.cpp
)

add_executable(devices ${SOURCE_LIST})

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

target_link_libraries(devices Qt5::Core Qt5::Network)
