#include <QCoreApplication>
#include "HARDWARE/AD5313_DAC/ad5313.h"
#include "HARDWARE/MCP23S17/mcp23s17.h"
#include "HARDWARE/encoder.h"
#include "HARDWARE/GPIO/GPIO.h"

using namespace exploringBB;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    system("/home/szymon/mgr/init.sh");

//    AD5313 *dac = new AD5313();
//    dac->write_voltage(3.f,1);
//    dac->write_voltage(2.5f,1);
    GPIO *test = new GPIO(49);

    MCP23S17 *expander = new MCP23S17(2,0,1000000,SPIDevice::MODE1);
    unsigned char regs[2];
    expander->read_registers(MCP23S17::GPIOA, 2, regs);
//    test->setDirection(OUTPUT);
//    test->setValue(HIGH);
//    test->setDirection(INPUT);
    ofstream fs;
    fs.open(("/sys/class/gpio/gpio49/direction"));
    if (!fs.is_open()){
        perror("GPIO: write failed to open file ");
        return -1;
    }
    fs << "out";
    fs.close();
    expander->read_registers(MCP23S17::IODIRA, 2, regs);
    expander->read_registers(MCP23S17::GPIOA, 2, regs);

    Encoder *test1 = new Encoder();
    test1->reset();
    unsigned char val;
    int val2, val3;
//    test1->read_byte(Encoder::CHART, Encoder::MORE, val);
    test1->read_values(val2, val3);
    //return a.exec();
    return 0;
}

