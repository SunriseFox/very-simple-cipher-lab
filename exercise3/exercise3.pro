TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    bright.h \
    knownprimes.h \
    mathutils.h \
    random.h \
    rsa.h

SOURCES += \
    main.cpp \
    bright.cpp \
    knownprimes.cpp \
    mathutils.cpp \
    random.cpp \
    rsa.cpp
