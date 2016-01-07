#include "ad5313.h"
#include <string.h>

AD5313::AD5313()
{
    device = new SPIDevice(2,1);
    device->setSpeed(400000);
    device->setMode(SPIDevice::MODE0);
    memset(tx_buf, 0, sizeof(tx_buf));
}

AD5313::AD5313(unsigned int bus, unsigned int num_of_device, unsigned int speed, SPIDevice::SPIMODE mode)
{
    device = new SPIDevice(bus,num_of_device);
    device->setSpeed(speed);
    device->setMode(mode);
    memset(tx_buf, 0, sizeof(tx_buf));
}
int AD5313::write_voltage(float voltage, int channel)
{
    if (voltage<0.0f || voltage>5.0f){
        return -1;
    }

    unsigned int reg = (unsigned int) ((voltage*1023)/5);
    if (channel == 2){
        channel = 8;
    }
    tx_buf[0] = 0b00110000 | channel;
    tx_buf[1] = (unsigned char) (reg >> 2);
    tx_buf[2] = (unsigned char) (reg << 6);

    return device->write(tx_buf, 3);
}

