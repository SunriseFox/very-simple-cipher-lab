TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    caesar.cpp \
    monoalphabetic.cpp \
    random.cpp

HEADERS += \
    caesar.h \
    monoalphabetic.h \
    random.h
