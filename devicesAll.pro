TEMPLATE = subdirs

CONFIG += c++11

SUBDIRS = \
    program \
    test

tests.depends = program
