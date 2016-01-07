#include <QCoreApplication>
#include "HARDWARE/AD5313_DAC/ad5313.h"
#include "HARDWARE/MCP23S17/mcp23s17.h"
#include "HARDWARE/encoder.h"

using namespace exploringBB;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    system("/home/szymon/mgr/init.sh");

//    AD5313 *dac = new AD5313();
//    dac->write_voltage(5.f,2);
//    dac->write_voltage(4.5f,1);
//    MCP23S17 *expander = new MCP23S17(2,0,1000000,SPIDevice::MODE1);
    unsigned char reg;
    unsigned char regs[15];
//    memset(regs, 0, sizeof regs);
////    expander->read_register(MCP23S17::IODIRB,reg);

//    expander->write_register(MCP23S17::IODIRA,0x0f);
//    expander->write_register(MCP23S17::OLATA, 0xf0);
//    expander->read_registers(MCP23S17::GPIOA, 2, regs);
//    expander ->read_register(MCP23S17::GPIOA, reg);
    Encoder *test1 = new Encoder();
    test1->reset();
    unsigned char val;
    int val2, val3;
    test1->read_byte(Encoder::CHART, Encoder::MORE, val);
    test1->read_values(val2, val3);
    //return a.exec();
    return 0;
}

