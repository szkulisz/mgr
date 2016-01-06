#include "mcp23s17.h"

MCP23S17::MCP23S17()
{

}

int MCP23S17::write_register(unsigned char adress, unsigned char value)
{
    if(adress < IODIRA || adress > OLATB){
        return -1;
    }

}

