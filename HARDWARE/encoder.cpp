#include "encoder.h"
#include <unistd.h>

using namespace exploringBB;

Encoder::Encoder()
{
    mExpander = new MCP23S17(2,0,1000000,SPIDevice::MODE1);
    mPinEnable = new GPIO(49);
    mPinReset = new GPIO(14);
    mPinSelect = new GPIO(115);
    mPinReset-> setDirection(OUTPUT);
    mPinEnable-> setDirection(OUTPUT);
    mPinSelect-> setDirection(OUTPUT);
    reset();
    release();
}

void Encoder::reset()
{
    mPinReset->setValue(HIGH);
    usleep(10);
    mPinReset->setValue(LOW);
}

int Encoder::readValues(int &cart, int &pend)
{
    static unsigned char cartMore, cartLess, pendMore, pendLess;
    static int cartPrev = 0;

    release();
    readBytes(MORE, cartMore, pendMore);
    readBytes(LESS, cartLess, pendLess);

    cart = cartMore;
    cart <<= 8;
    cart += cartLess;

    if (cart & 0x00008000) {
        cart |= 0xFFFF8000;
    }
    if ((cart - cartPrev) > 300){
        cart = cart - 514;
    } else {
        if ((cart - cartPrev) < -300)
            cart = cart + 514;
    }

    cartPrev = cart;

    pend = pendMore;
    pend <<= 8;
    pend += pendLess;

    if (pend & 0x00008000) {
        pend |= 0xFFFF8000;
    }

    release();
    return 0;
}

void Encoder::release()
{
    mPinEnable-> setValue(LOW);
    mPinSelect-> setValue(LOW);
}

int Encoder::readBytes(Encoder::BYTE byte, unsigned char &cart, unsigned char &pend)
{
    static unsigned char valuesert[2];
    static int ret;
//    memset(values, 0, sizeof(values));

    mPinEnable-> setValue(HIGH);
    if(byte == MORE){
        mPinSelect-> setValue(HIGH);
    }else{
        mPinSelect-> setValue(LOW);
    }
    ret = this->mExpander->readRegisters(MCP23S17::GPIOA, 2, valuesert);
    if (ret<0){
        valuesert[0] = 0xFF;
        valuesert[1] = 0xFF;
    }
//    release();
    cart = valuesert[0];
    pend = valuesert[1];
    return ret;

}

int Encoder::readByte(Encoder::WHICH which, Encoder::BYTE byte, unsigned char &value)
{
    unsigned char reg;

//    release();
    mPinEnable-> setValue(HIGH);
    if(byte == MORE){
        mPinSelect-> setValue(HIGH);
    }else{
        mPinSelect-> setValue(LOW);
    }
    if (which == CHART){
        reg = MCP23S17::GPIOA;
    }else{
        reg = MCP23S17::GPIOB;
    }
    int ret = mExpander->readRegister(reg, value);
    if (ret<0){
        value = 0xFF;
    }
//    release();
    return ret;

}

