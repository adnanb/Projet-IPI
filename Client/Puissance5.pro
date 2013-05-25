#-------------------------------------------------
#
# Project created by QtCreator 2013-04-23T22:25:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Puissance5
TEMPLATE = app


SOURCES +=\
    Board.cpp \
    Boardview.cpp \
    Core.cpp \
    Game.cpp \
    Main.cpp \
    Mainwindow.cpp \
    StringUtils.inl \
    Network.cpp \
    Player.cpp \
    Square.cpp \
    Singleton.cpp

HEADERS  += \
    Board.hpp \
    Boardview.hpp \
    Core.hpp \
    Game.hpp \
    Mainwindow.hpp \
    Network.hpp \
    Player.hpp \
    Square.hpp \
    StringUtils.hpp \
    Singleton.hpp

FORMS    += mainwindow.ui
