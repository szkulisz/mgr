#include <QCoreApplication>
#include "AD5313_DAC/ad5313.h"
using namespace exploringBB;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    system("/home/szymon/mgr/init.sh");

    AD5313 *DAC = new AD5313();
//    DAC->write_voltage(5.f,2);
//    DAC->write_voltage(4.5f,1);
    //return a.exec();
    return 0;
}

