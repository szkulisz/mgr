#include <QCoreApplication>
#include "HARDWARE/AD5313_DAC/ad5313.h"
#include "HARDWARE/MCP23S17/mcp23s17.h"

using namespace exploringBB;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    system("/home/szymon/mgr/init.sh");

//    AD5313 *dac = new AD5313();
//    dac->write_voltage(5.f,2);
//    dac->write_voltage(4.5f,1);
    MCP23S17 *expander = new MCP23S17();
    unsigned char reg;
    unsigned char regs[15];
    memset(regs, 0, sizeof regs);
//    expander->read_register(IODIRB,reg);

    expander->write_register(IODIRA,0x0f);
    expander->write_register(OLATA, 0xf0);
    expander->read_registers(0x00,15,regs);
    //return a.exec();
    return 0;
}

