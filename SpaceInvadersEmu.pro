#-------------------------------------------------
#
# Project created by QtCreator 2016-01-24T11:00:21
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpaceInvadersEmu
TEMPLATE = app

DESTDIR=build

QMAKE_CXXFLAGS += -Wextra

INCLUDEPATH += include

SOURCES += src/screen.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/opcodedefinition.cpp \
    src/iomanager.cpp \
    src/memorymanager.cpp \
    src/cpu.cpp \
    src/utility.cpp \
    src/cpustate.cpp

HEADERS += include/screen.hpp \
   include/mainwindow.hpp \
    include/common.hpp \
    include/cpustate.hpp \
    include/opcodedefinition.hpp \
    include/utility.hpp \
    include/iomanager.hpp \
    include/memorymanager.hpp \
    include/cpu.hpp \
    include/nano-signal/nano_function.hpp \
    include/nano-signal/nano_observer.hpp \
    include/nano-signal/nano_signal_slot.hpp \
    include/opcodes.hpp

FORMS    += ui/mainwindow.ui

RESOURCES += \
    style.qrc
