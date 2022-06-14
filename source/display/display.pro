#-------------------------------------------------
#
# Project created by QtCreator 2021-09-25T16:23:55
#
#-------------------------------------------------

QT       += core gui network multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = display
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    config.cpp \
        main.cpp \
    glwidget.cpp \
    serialworker.cpp \
    window.cpp \
    painter.cpp \
    vehiclevalues.cpp \
    indicators.cpp \
    hwdialog.cpp

HEADERS += \
    config.h \
    glwidget.h \
    serialcontroller.h \
    serialworker.h \
    window.h \
    painter.h \
    images.h \
    variables.h \
    drawing.h \
    converters.h \
    vehiclevalues.h \
    indicators.h \
    hwdialog.h

FORMS += \
    hwdialog.ui

RESOURCES += \
    resources.qrc

target.path += /root
INSTALLS += target
