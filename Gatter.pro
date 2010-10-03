#-------------------------------------------------
#
# Project created by QtCreator 2010-08-27T19:23:56
#
#-------------------------------------------------

QT       += core gui

TARGET = Gatter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scene.cpp \
    element.cpp \
    gatter.cpp \
    connection.cpp \
    switch.cpp \
    lamp.cpp \
    button.cpp \
    clock.cpp \
    subscene.cpp \
    colorbutton.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    scene.h \
    element.h \
    gatter.h \
    connection.h \
    defines.h \
    switch.h \
    lamp.h \
    button.h \
    clock.h \
    subscene.h \
    colorbutton.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
TRANSLATIONS += gatter_de_DE.ts
