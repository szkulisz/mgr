#ifndef MCP23S17_H
#define MCP23S17_H

#include "HARDWARE/SPI/SPIDevice.h"
using namespace exploringBB;

class MCP23S17
{
public:
    enum REG{IODIRA,IODIRB,IPOLA,IPOLB,GPINTENA,GPINTENB,DEFVALA,
             DEFVALB,INTCONA,INTCONB,IOCON,IOCONB,GPPUA,GPPUB,
             INTFA,INTFB,INTCAPA,INTCAPB,GPIOA,GPIOB,OLATA,OLATB};

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
