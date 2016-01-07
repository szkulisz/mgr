#include "enkodery.h"

Enkodery::Enkodery()
{
    expander = new MCP23S17(2,0,1000000,SPIDevice::MODE1);
}

void Enkodery::reset()
{

}

