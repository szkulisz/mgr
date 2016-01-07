#ifndef AD5313_H
#define AD5313_H

#include "HARDWARE/SPI/SPIDevice.h"

using namespace exploringBB;

/**
 * @brief The AD5313 class
 *  klasa obsługująca AD5313 (10bit, podwójne DAC)
 *  komunikacja po SPI
 */
class AD5313
{
public:
    AD5313();

    AD5313(unsigned int bus, unsigned int device, unsigned int speed, SPIDevice::SPIMODE mode);

    /**
     * @brief write_voltage
     *  uaktualnia wystawiane napięcie na odpowiednim kanale
     * @param voltage
     *  zakres: 0-5
     * @param channel
     *  1:kanał A
     *  2:kanał B
     * @return
     *  -1: błąd zakresu
     *  0:  ok
     */
    int write_voltage(float voltage, int channel);


private:
    SPIDevice *device;
    unsigned char tx_buf[3] = {0,0,0};
};

#endif // AD5313_H
