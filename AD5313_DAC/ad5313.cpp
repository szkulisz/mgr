#include "ad5313.h"
#include <string.h>


AD5313::AD5313()
{
    device = new SPIDevice(2,1);
    device->setSpeed(1000000);
    device->setMode(SPIDevice::MODE1);
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

    device->write(tx_buf, 3);
    return 0;
}

