#ifndef ENCODER_H
#define ENCODER_H

#include "MCP23S17/mcp23s17.h"
#include "GPIO/GPIO.h"
using namespace exploringBB;

class Encoder
{
public:
    enum WHICH{CHART, PEND};
    enum BYTE{MORE, LESS}; //significant

    Encoder();

    void reset();
    int read_values(int &chart, int &pend);


//private:
    MCP23S17 *expander;
    GPIO *pin_reset;
    GPIO *pin_enable;
    GPIO *pin_select;

    void release();
    int read_bytes(BYTE byte, unsigned char &chart, unsigned char &pend);
    int read_byte(WHICH which, BYTE byte, unsigned char &value);
};

#endif // ENCODER_H
