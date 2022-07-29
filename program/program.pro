TEMPLATE = app

VERSION = 10
TARGET = devices

CONFIG += console no_lflags_merge rtti_off warn_on c++latest strict_c++ precompile_header utf8_source
CONFIG -= app_bundle cmdline qml_debug no_include_pwd qtquickcompiler

DEFINES +=                 \
  QT_DISABLE_DEPRECATED_BEFORE=0x050B00 \# отключить API, устаревший в 5.11 или более ранних версиях
  QT_USE_FAST_CONCATENATION\
  QT_DEPRECATED_WARNINGS   \
  QT_USE_FAST_OPERATOR_PLUS\
  QT_NO_URL_CAST_FROM_STRING\
  QT_USE_STRING_BUILDER \
  QT_NO_CAST_TO_ASCII \
  QT_NO_CAST_FROM_ASCII \
  QT_NO_NARROWING_CONVERSIONS_IN_CONNECT \
  QT_NO_FOREACH \
  QT_STRICT_ITERATORS

include(build.pri)

SOURCES += \
       $$PWD/main.cpp \
