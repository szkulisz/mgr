#include <QCoreApplication>
#include "SPI/SPIDevice.h"
using namespace exploringBB;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SPIDevice *DAC = new SPIDevice(2,1);
    DAC->setSpeed(400000);
    DAC->setMode(SPIDevice::MODE1);
   // DAC->setBitsPerWord(24);
    unsigned char send[3],receive[3];
    send[0] = 0b00110001; //write to and update DAC Channel A
    send[1] = 0b00000000;
    send[2] = 0b00000000;
    DAC->transfer(send,receive, 3);

    return a.exec();
}

