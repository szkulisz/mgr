#ifndef WAHADLO_H
#define WAHADLO_H

#include "HARDWARE/encoder.h"
#include "HARDWARE/AD5313_DAC/ad5313.h"

class Wahadlo
{
public:
    Wahadlo();

    int steruj(float u);
    void odczytaj_pozycje(int &wozek, int &wahadlo);


    Encoder *s_encoder;
    AD5313 *s_dac;
    int s_pozycja_wozka;
    int s_pozycja_wahadla;

    int getS_pozycja_wahadla() const;
    int getS_pozycja_wozka() const;
    void odczytaj_pozycje();
};

#endif // WAHADLO_H
