TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    config.cpp \
    logger.cpp \
    server.cpp

HEADERS += \
    config.h \
    colors.h \
    logging.h \
    log_levels.h \
    logger.h \
    server.h

DISTFILES += \
    ../build-OOP_Server-Desktop_Qt_5_10_0_GCC_64bit-Debug/server.conf
