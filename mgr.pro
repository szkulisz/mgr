QT += core sql network
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
    HARDWARE/encoder.cpp \
    regulatory/regulator.cpp \
    regulatory/regulatorp.cpp \
    regulatory/regulatorpid.cpp \
    regulatory/dekorator_sygnalu.cpp \
    objekt_siso.cpp \
    petla.cpp \
    odczyt_xml.cpp \
    tinyxml2.cpp \
    wahadlo.cpp \
    program.cpp

HEADERS += \
    HARDWARE/PWM/pwm.h \
    HARDWARE/PWM/util.h \
    HARDWARE/SPI/SPIDevice.h \
    HARDWARE/SPI/BusDevice.h \
    HARDWARE/AD5313_DAC/ad5313.h \
    HARDWARE/MCP23S17/mcp23s17.h \
    HARDWARE/GPIO/GPIO.h \
    HARDWARE/encoder.h \
    regulatory/regulator.h \
    regulatory/regulatorp.h \
    regulatory/regulatorpid.h \
    regulatory/dekorator_sygnalu.h \
    objekt_siso.h \
    petla.h \
    odczyt_xml.h \
    tinyxml2.h \
    wahadlo.h \
    program.h


QMAKE_CXXFLAGS += -pthread
QMAKE_CFLAGS += -pthread
LIBS += -pthread
