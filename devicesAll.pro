TEMPLATE = subdirs

CONFIG += debug_and_release c++11

CONFIG(debug, debug|release) {
    message("DEBUG")
    SUBDIRS = \
        program \
        test

    tests.depends = program
} else {
    message("RELEASE")
    SUBDIRS = \
        program
}


