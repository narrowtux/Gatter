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
    settingsdialog.cpp \
    subscenechoosedialog.cpp \
    subsceneinfo.cpp \
    delay.cpp \
    loadthread.cpp \
    flipflop.cpp \
    enumcombobox.cpp \
    hexoutput.cpp \
    application.cpp \
    graphicsview.cpp \
    elementcatalog.cpp \
    catalogitem.cpp

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
    settingsdialog.h \
    subscenechoosedialog.h \
    subsceneinfo.h \
    delay.h \
    loadthread.h \
    flipflop.h \
    enumcombobox.h \
    hexoutput.h \
    application.h \
    undoactions.h \
    graphicsview.h \
    elementcatalog.h \
    catalogitem.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    subscenechoosedialog.ui
TRANSLATIONS += gatter_de_DE.ts
ICON = Gatter.icns

RESOURCES += \
    res.qrc
