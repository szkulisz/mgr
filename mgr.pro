QT += core
QT -= gui

TARGET = mgr
    target.files = mgr
    target.path = /home/szymon/mgr/mgr_program
INSTALLS += target

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    PWM/pwm.cpp \
    PWM/util.cpp \
    SPI/SPIDevice.cpp \
    SPI/BusDevice.cpp \
    AD5313_DAC/ad5313.cpp \
    MCP23S17/mcp23s17.cpp

HEADERS += \
    PWM/pwm.h \
    PWM/util.h \
    SPI/SPIDevice.h \
    SPI/BusDevice.h \
    ad5313.h \
    AD5313_DAC/ad5313.h \
    mcp23s17.h \
    MCP23S17/mcp23s17.h

RESOURCES += \
    mcp23s17.qrc

