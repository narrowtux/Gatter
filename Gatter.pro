#-------------------------------------------------
#
# Project created by QtCreator 2010-08-27T19:23:56
#
#-------------------------------------------------

QT       += core gui

TARGET = Gatter
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/scene/scene.cpp \
    src/connection.cpp \
    src/colorbutton.cpp \
    src/settingsdialog.cpp \
    src/subscenechoosedialog.cpp \
    src/subsceneinfo.cpp \
    src/loadthread.cpp \
    src/enumcombobox.cpp \
    src/elements/hexoutput.cpp \
    src/application.cpp \
    src/scene/graphicsview.cpp \
    src/catalog/elementcatalog.cpp \
    src/catalog/catalogitem.cpp \
    src/elements/element.cpp \
    src/elements/gatter.cpp \
    src/elements/switch.cpp \
    src/elements/lamp.cpp \
    src/elements/button.cpp \
    src/elements/clock.cpp \
    src/elements/subscene.cpp \
    src/elements/delay.cpp \
    src/elements/flipflop.cpp

HEADERS  += src/mainwindow.h \
    src/scene/scene.h \
    src/elements/element.h \
    src/elements/gatter.h \
    src/connection.h \
    src/defines.h \
    src/elements/switch.h \
    src/elements/lamp.h \
    src/elements/button.h \
    src/elements/clock.h \
    src/elements/subscene.h \
    src/colorbutton.h \
    src/settingsdialog.h \
    src/subscenechoosedialog.h \
    src/subsceneinfo.h \
    src/elements/delay.h \
    src/loadthread.h \
    src/elements/flipflop.h \
    src/enumcombobox.h \
    src/elements/hexoutput.h \
    src/application.h \
    src/undoactions.h \
    src/scene/graphicsview.h \
    src/catalog/elementcatalog.h \
    src/catalog/catalogitem.h

FORMS    += src/mainwindow.ui \
    src/settingsdialog.ui \
    src/subscenechoosedialog.ui
TRANSLATIONS += gatter_de_DE.ts
ICON = Gatter.icns

RESOURCES += \
    src/res.qrc
