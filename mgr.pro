QT += core
QT -= gui

TARGET = mgr
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    PWM/pwm.cpp \
    PWM/util.cpp \
    SPI/SPIDevice.cpp

HEADERS += \
    PWM/pwm.h \
    PWM/util.h \
    SPI/SPIDevice.h

