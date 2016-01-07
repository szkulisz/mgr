#include "encoder.h"
#include <unistd.h>

Encoder::Encoder()
{
    expander = new MCP23S17(2,0,1000000,SPIDevice::MODE1);
    pin_reset = new GPIO(14);
    pin_enable = new GPIO(49);
    pin_select = new GPIO(115);
    pin_reset-> setDirection(OUTPUT);
    pin_enable-> setDirection(OUTPUT);
    pin_select-> setDirection(OUTPUT);
    release();
    reset();
}

void Encoder::reset()
{
    pin_reset->setValue(HIGH);
    usleep(1);
    pin_reset->setValue(LOW);
}

int Encoder::read_values(int &chart, int &pend)
{
    unsigned char chart_more, chart_less, pend_more, pend_less;

    release();
    read_bytes(MORE, chart_more, pend_more);
    read_bytes(LESS, chart_less, pend_less);

    chart = chart_more;
    chart <<= 8;
    chart += chart_less;

    if (chart & 0x00008000) {
        chart |= 0xFFFF8000;
    }
    pend = pend_more;
    pend <<= 8;
    pend += pend_less;

    if (pend & 0x00008000) {
        pend |= 0xFFFF8000;
    }

    release();
    return 0;
}

void Encoder::release()
{
    pin_enable-> setValue(LOW);
    pin_select-> setValue(LOW);
}

int Encoder::read_bytes(Encoder::BYTE byte, unsigned char &chart, unsigned char &pend)
{
    unsigned char values[2];
//    memset(values, 0, sizeof(values));

    pin_enable-> setValue(HIGH);
    if(byte == MORE){
        pin_select-> setValue(HIGH);
    }else{
        pin_select-> setValue(LOW);
    }
    int ret = expander->read_registers(MCP23S17::GPIOA, 2, values);
    if (ret<0){
        values[0] = 0xFF;
        values[1] = 0xFF;
    }
//    release();
    chart = values[0];
    pend = values[0];
    return ret;

}

int Encoder::read_byte(Encoder::WHICH which, Encoder::BYTE byte, unsigned char &value)
{
    unsigned char reg;

//    release();
    pin_enable-> setValue(HIGH);
    if(byte == MORE){
        pin_select-> setValue(HIGH);
    }else{
        pin_select-> setValue(LOW);
    }
    if (which == CHART){
        reg = MCP23S17::GPIOA;
    }else{
        reg = MCP23S17::GPIOB;
    }
    int ret = expander->read_register(reg, value);
    if (ret<0){
        value = 0xFF;
    }
//    release();
    return ret;

}

