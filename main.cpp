#include <QCoreApplication>
#include <QTimer>
#include "HARDWARE/AD5313_DAC/ad5313.h"
#include "HARDWARE/MCP23S17/mcp23s17.h"
#include "HARDWARE/encoder.h"
#include "HARDWARE/GPIO/GPIO.h"
#include "petla.h"
#include "regulatory/regulatorpid.h"
#include <map>
#include "program.h"

using namespace exploringBB;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    system("/home/szymon/mgr/init.sh");

//    AD5313 *dac = new AD5313();
//    dac->write_voltage(3.f,1);
//    dac->write_voltage(2.5f,1);

//    MCP23S17 *expander = new MCP23S17(2,0,1000000,SPIDevice::MODE1);
//    unsigned char regs[2];
//    expander->read_registers(MCP23S17::GPIOA, 2, regs);
//    expander->read_registers(MCP23S17::IODIRA, 2, regs);
//    expander->read_registers(MCP23S17::GPIOA, 2, regs);

//    Encoder *test1 = new Encoder();
//    test1->reset();
//    int val2, val3;
//    test1->read_values(val2, val3);


    Program program;

    return a.exec();
//    return 0;
}

