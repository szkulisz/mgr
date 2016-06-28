#ifndef PETLA_H
#define PETLA_H

#include <memory>
#include "regulatory/regulator.h"
#include "pendulum.h"
#include <string>
#include <deque>

class Sygnal;
/**
 * @brief The Petla class - implementacja pętli symulacji
 *          wymaga przekazania utworzonego obiektu i regulatora
 */
class Petla
{
public:
    Petla( Regulator *);
    ~Petla();

    void krok();

    void ustaw_parametry_regulatora(){ s_reg1->ustaw_parametry(); }
    void ustaw_parametry_regulatora(std::map<std::string, float> parametry){ s_reg1->ustaw_parametry(parametry); }
    void zapisz_parametry_regulatora(){ s_reg1->zapisz_parametry(); }
    void ustaw_wartosc_zadana(Sygnal *a){ s_reg1->ustaw_wartosc_zadana(a);}
    void ustaw_wartosc_zadana(float a){ s_reg1->ustaw_wartosc_zadana(a);}
    float odczytaj_wartosc_zadana() const { return s_reg1->odczytaj_wartosc_zadana(); }
    float odczytaj_uchyb() const { return s_reg1->odczytaj_uchyb(); }
    float odczytaj_wyjscie_obiektu() const { return s_reg1->odczytaj_wyjscie_obiektu(); }
    float odczytaj_sterowanie() const { return s_reg1->odczytaj_sterowanie(); }
    void wypisz_parametry_regulatora() const { s_reg1->wypisz_parametry(); }

    void nowy_regulator(Regulator *reg);

    int s_aktualne_wyjscie = 0;
    int mPhase=0;
//private:
//    std::unique_ptr< Regulator > s_reg1;
    Regulator *s_reg1;
    Pendulum *wahadlo;

};

#endif // PETLA_H
