#ifndef MCP23S17_H
#define MCP23S17_H

#include "HARDWARE/SPI/SPIDevice.h"
using namespace exploringBB;

#define IODIRA 0x00
#define IPOLA 0x02
#define GPINTENA 0x04
#define DEFVALA 0x06
#define INTCONA 0x08
#define IOCON 0x0A
#define GPPUA 0x0C
#define INTFA 0x0E
#define INTCAPA 0x10
#define GPIOA 0x12
#define OLATA 0x14
#define IODIRB 0x01
#define IPOLB 0x03
#define GPINTENB 0x05
#define DEFVALB 0x07
#define INTCONB 0x09
//#define IOCON 0x0B
#define GPPUB 0x0D
#define INTFB 0x0F
#define INTCAPB 0x11
#define GPIOB 0x13
#define OLATB 0x15

class MCP23S17
{
public:
    MCP23S17();
    MCP23S17(unsigned int bus, unsigned int num_of_device, unsigned int speed, SPIDevice::SPIMODE mode);

    int write_register(unsigned char adress, unsigned char value);
    int read_register(unsigned char adress, unsigned char &value);
    int read_registers(unsigned char adress_of_first_reg, int number_of_reg, unsigned char *reg);

private:
    SPIDevice *device;
    unsigned char tx_buf[25];
};

#endif // MCP23S17_H
