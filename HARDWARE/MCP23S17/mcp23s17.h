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
    MCP23S17(unsigned int bus, unsigned int numOfDevice, unsigned int speed, SPIDevice::SPIMODE mode);

    int writeRegister(unsigned char adress, unsigned char value);
    int readRegister(unsigned char adress, unsigned char &value);
    int readRegisters(unsigned char firstRegisterAdress, int numberOfRegisters, unsigned char *reg);

private:
    SPIDevice *mDevice;
    unsigned char mTxBuf[25];
};

#endif // MCP23S17_H
