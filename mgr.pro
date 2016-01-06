QT += core
QT -= gui

TARGET = mgr
target.files = mgr
target.path = home/szymon/mgr/mgr
INSTALLS += target
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    PWM/pwm.cpp \
    PWM/util.cpp \
    SPI/SPIDevice.cpp \
    SPI/BusDevice.cpp

HEADERS += \
    PWM/pwm.h \
    PWM/util.h \
    SPI/SPIDevice.h \
    SPI/BusDevice.h

