#include "mcp23s17.h"
#include <string.h>
#include <cstring>

MCP23S17::MCP23S17()
{
    device = new SPIDevice(2,0);
    device->setSpeed(400000);
    device->setMode(SPIDevice::MODE1);
    memset(tx_buf, 0, sizeof(tx_buf));
}

MCP23S17::MCP23S17(unsigned int bus, unsigned int num_of_device, unsigned int speed, SPIDevice::SPIMODE mode)
{
    device = new SPIDevice(bus,num_of_device);
    device->setSpeed(speed);
    device->setMode(mode);
    memset(tx_buf, 0, sizeof(tx_buf));
}

int MCP23S17::write_register(unsigned char adress, unsigned char value)
{
    if(adress > OLATB){
        return -2;
    }

    tx_buf[0] = 0b01001110;
    tx_buf[1] = adress;
    tx_buf[2] = value;

    return device->write(tx_buf, 3);
}

int MCP23S17::read_register(unsigned char adress, unsigned char &reg)
{
    if(adress > OLATB){
        return -2;
    }
    unsigned char receive[3];
    memset(receive, 0, sizeof receive);

    tx_buf[0] = 0b01001111;
    tx_buf[1] = adress;

    int status = device->transfer(tx_buf, receive, 3);
    if (status <= 0){
        return status;
    }

    reg = receive[3];
    return 0;
}

int MCP23S17::read_registers(unsigned char adress_of_first_reg, int number_of_reg, unsigned char *reg)
{
    if(adress_of_first_reg > OLATB){
        return -2;
    }

    unsigned char receive[number_of_reg + 2];
    memset(receive, 0, sizeof receive);

    tx_buf[0] = 0b01001111;
    tx_buf[1] = adress_of_first_reg;

    int status = device->transfer(tx_buf, receive, number_of_reg + 2);
    if (status <= 0){
        return status;
    }

    memcpy(reg, receive+2, number_of_reg);
    return 0;
}

