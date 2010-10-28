#-------------------------------------------------
#
# Project created by QtCreator 2010-08-27T19:23:56
#
#-------------------------------------------------

QT       += core gui

TARGET = Gatter
TEMPLATE = app


SOURCES += src/main.cpp\
        src/widgets/mainwindow.cpp \
    src/scene/scene.cpp \
    src/connection.cpp \
    src/widgets/colorbutton.cpp \
    src/widgets/settingsdialog.cpp \
    src/widgets/subscenechoosedialog.cpp \
    src/subsceneinfo.cpp \
    src/loadthread.cpp \
    src/widgets/enumcombobox.cpp \
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
    src/elements/flipflop.cpp \
    src/elements/distributor.cpp \
    src/connectionline.cpp

HEADERS  += src/widgets/mainwindow.h \
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
    src/widgets/colorbutton.h \
    src/widgets/settingsdialog.h \
    src/widgets/subscenechoosedialog.h \
    src/subsceneinfo.h \
    src/elements/delay.h \
    src/loadthread.h \
    src/elements/flipflop.h \
    src/widgets/enumcombobox.h \
    src/elements/hexoutput.h \
    src/application.h \
    src/undoactions.h \
    src/scene/graphicsview.h \
    src/catalog/elementcatalog.h \
    src/catalog/catalogitem.h \
    src/elements/distributor.h \
    src/connectionline.h

FORMS    += src/widgets/mainwindow.ui \
    src/widgets/settingsdialog.ui \
    src/widgets/subscenechoosedialog.ui
TRANSLATIONS += gatter_de_DE.ts
ICON = Gatter.icns

RESOURCES += \
    src/res.qrc
