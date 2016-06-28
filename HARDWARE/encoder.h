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
    int readValues(int &cart, int &pend);


//private:
    MCP23S17 *mExpander;
    GPIO *mPinReset;
    GPIO *mPinEnable;
    GPIO *mPinSelect;

    void release();
    int readBytes(BYTE byte, unsigned char &cart, unsigned char &pend);
    int readByte(WHICH which, BYTE byte, unsigned char &value);
};

#endif // ENCODER_H
