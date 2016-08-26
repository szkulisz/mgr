#include "mcp23s17.h"
#include <string.h>
#include <cstring>

MCP23S17::MCP23S17()
{
    mDevice = new SPIDevice(2,0);
    mDevice->setSpeed(400000);
    mDevice->setMode(SPIDevice::MODE1);
    memset(mTxBuf, 0, sizeof(mTxBuf));
}

MCP23S17::MCP23S17(unsigned int bus, unsigned int numOfDevice, unsigned int speed, SPIDevice::SPIMODE mode)
{
    mDevice = new SPIDevice(bus,numOfDevice);
    mDevice->setSpeed(speed);
    mDevice->setMode(mode);
    memset(mTxBuf, 0, sizeof(mTxBuf));
}

int MCP23S17::writeRegister(unsigned char adress, unsigned char value)
{
    if(adress > OLATB){
        return -2;
    }

    mTxBuf[0] = 0b01001110;
    mTxBuf[1] = adress;
    mTxBuf[2] = value;

    return mDevice->write(mTxBuf, 3);
}

int MCP23S17::readRegister(unsigned char adress, unsigned char &reg)
{
    if(adress > OLATB){
        return -2;
    }
    unsigned char receive[3];
    memset(receive, 0, sizeof receive);

    mTxBuf[0] = 0b01001111;
    mTxBuf[1] = adress;
    mTxBuf[2] = 0;

    int status = mDevice->transfer(mTxBuf, receive, 3);
    if (status <= 0){
        return status;
    }

    reg = receive[2];
    return 0;
}

int MCP23S17::readRegisters(unsigned char firstRegisterAdress, int numberOfRegisters, unsigned char *reg)
{
    if(firstRegisterAdress > OLATB){
        return -2;
    }

    unsigned char receive[5 + 2];
    memset(receive, 0, sizeof receive);

    mTxBuf[0] = 0b01001111;
    mTxBuf[1] = firstRegisterAdress;

    int status = mDevice->transfer(mTxBuf, receive, numberOfRegisters + 2);
    if (status <= 0){
        return status;
    }

    memcpy(reg, receive+2, numberOfRegisters);
    return 0;
}

