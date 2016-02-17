#-------------------------------------------------
#
# Project created by QtCreator 2016-01-24T11:00:21
#
#-------------------------------------------------

QT       += core gui multimedia


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpaceInvadersEmu
TEMPLATE = app

DESTDIR=build

QMAKE_CXXFLAGS += -Wextra -O2 -std=c++14

INCLUDEPATH += include

SOURCES += src/screen.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/opcodedefinition.cpp \
    src/iomanager.cpp \
    src/memorymanager.cpp \
    src/cpu.cpp \
    src/utility.cpp \
    src/cpustate.cpp \
    src/invaders/shiftregister.cpp \
    src/motherboard.cpp \
    src/invaders/invadersboard.cpp \
    src/invaders/soundmanager.cpp

HEADERS += include/screen.hpp \
   include/mainwindow.hpp \
    include/common.hpp \
    include/cpustate.hpp \
    include/opcodedefinition.hpp \
    include/utility.hpp \
    include/iomanager.hpp \
    include/memorymanager.hpp \
    include/cpu.hpp \
    include/opcodes.hpp \
    include/invaders/shiftregister.hpp \
    include/motherboard.hpp \
    include/invaders/invadersboard.hpp \
    include/invaders/soundmanager.hpp \
    include/array_view/array_view.hpp \
    include/noncopyable.hpp

FORMS    += ui/mainwindow.ui

RESOURCES += \
    style.qrc
