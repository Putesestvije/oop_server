TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LFLAGS += -pthread

SOURCES += main.cpp \
    config.cpp \
    logger.cpp \
    server.cpp \
    connection.cpp \
    logging.cpp \
    requesthandler.cpp \
    gethandler.cpp \
    newsstand.cpp \
    subscriber.cpp \
    deliveryttracker.cpp \
    typetracker.cpp \
    timetracker.cpp

HEADERS += \
    config.h \
    colors.h \
    logging.h \
    log_levels.h \
    logger.h \
    server.h \
    connection.h \
    request.h \
    requesthandler.h \
    gethandler.h \
    newsstand.h \
    subscriber.h \
    deliveryttracker.h \
    fifomessage.h \
    typetracker.h \
    timetracker.h

DISTFILES += \
    ../build-OOP_Server-Desktop_Qt_5_10_0_GCC_64bit-Debug/server.conf
