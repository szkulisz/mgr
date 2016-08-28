#include "ad5313.h"
#include <string.h>

AD5313::AD5313()
{
    mDevice = new SPIDevice(2,1);
    mDevice->setSpeed(400000);
    mDevice->setMode(SPIDevice::MODE1);
    memset(mTxBuf, 0, sizeof(mTxBuf));
}

AD5313::AD5313(unsigned int bus, unsigned int deviceNumber, unsigned int speed, SPIDevice::SPIMODE mode)
{
    mDevice = new SPIDevice(bus,deviceNumber);
    mDevice->setSpeed(speed);
    mDevice->setMode(mode);
    memset(mTxBuf, 0, sizeof(mTxBuf));
}
int AD5313::writeVoltage(float voltage, int channel)
{
    static unsigned int reg;

    if (voltage<0.0f || voltage>5.0f){
        return -1;
    }

    reg = (unsigned int) ((voltage*1023)/5);
    if (channel == 2){
        channel = 8;
    }
    mTxBuf[0] = 0b00110000 | channel;
    mTxBuf[1] = (unsigned char) (reg >> 2);
    mTxBuf[2] = (unsigned char) (reg << 6);

    return mDevice->write(mTxBuf, 3);
}

