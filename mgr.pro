QT += core
QT -= gui

TARGET = mgr
    target.files = mgr
    target.path = /home/szymon/mgr/mgr_program
INSTALLS += target

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    HARDWARE/PWM/pwm.cpp \
    HARDWARE/PWM/util.cpp \
    HARDWARE/SPI/SPIDevice.cpp \
    HARDWARE/SPI/BusDevice.cpp \
    HARDWARE/AD5313_DAC/ad5313.cpp \
    HARDWARE/MCP23S17/mcp23s17.cpp \
    HARDWARE/GPIO/GPIO.cpp \
    HARDWARE/encoder.cpp

HEADERS += \
    HARDWARE/PWM/pwm.h \
    HARDWARE/PWM/util.h \
    HARDWARE/SPI/SPIDevice.h \
    HARDWARE/SPI/BusDevice.h \
    HARDWARE/AD5313_DAC/ad5313.h \
    HARDWARE/MCP23S17/mcp23s17.h \
    HARDWARE/GPIO/GPIO.h \
    HARDWARE/encoder.h


QMAKE_CXXFLAGS += -pthread
QMAKE_CFLAGS += -pthread
LIBS += -pthread
