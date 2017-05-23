#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T22:59:36
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IOTer
TEMPLATE = app


SOURCES += main.cpp\
    SerialPort.cpp \
    PortSettingWidget.cpp \
    SettingWidget.cpp \
    AboutUsWidget.cpp \
    MainWidget.cpp \
    TemAndHumWidget.cpp \
    TemAndHumShowWidget.cpp \
    PasswordWidget.cpp

HEADERS  += \
    SerialPort.h \
    PortSettingWidget.h \
    SettingWidget.h \
    AboutUsWidget.h \
    MainWidget.h \
    TemAndHumWidget.h \
    TemAndHumShowWidget.h \
    PasswordWidget.h

RESOURCES += \
    images.qrc

OTHER_FILES += \
    images/w1.png \
    images/w2.png \
    images/w3.png

RC_FILE = ico.rc
