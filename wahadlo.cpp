#include "wahadlo.h"

Wahadlo::Wahadlo()
{
    s_encoder = new Encoder();
    s_dac = new AD5313();
}

int Wahadlo::steruj(float u)
{
    return s_dac->write_voltage(u + 2.5f, 1);
}

void Wahadlo::odczytaj_pozycje(int &wozek, int &wahadlo)
{
    s_encoder->read_values(wozek, wahadlo);
}

void Wahadlo::odczytaj_pozycje()
{
    s_encoder->read_values(s_pozycja_wozka, s_pozycja_wahadla);
}

int Wahadlo::getS_pozycja_wozka() const
{
    return s_pozycja_wozka;
}

int Wahadlo::getS_pozycja_wahadla() const
{
    return s_pozycja_wahadla;
}

