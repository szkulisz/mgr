#ifndef REGULATOR_H
#define REGULATOR_H

#include "objekt_siso.h"
#include <map>
#include <string>
#include <memory>
#include "dekorator_sygnalu.h"

class Odczyt_xml;

/**
 * @brief The Regulator class
 *          klasa definiująca interfejs regulatorów
 */
class Regulator : public Objekt_SISO {
public:
    Regulator(Odczyt_xml *x, std::string typ);
    Regulator(std::string typ);
    Regulator(std::map<std::string, float> parametry, std::string typ);
    virtual ~Regulator();

    float symuluj(float u) override = 0;
    void ustaw_wartosc_zadana(float a){ s_wartosc_zadana = a;}
    void ustaw_wartosc_zadana(Sygnal *a){ s_generator.reset(a); }
    virtual void ustaw_parametry();
    virtual void ustaw_parametry(std::map<std::string, float> parametry);
    void zapisz_parametry();
    float odczytaj_wartosc_zadana() const { return s_wartosc_zadana; }
    float odczytaj_uchyb() const { return s_uchyb; }
    float odczytaj_wyjscie_obiektu() const { return s_wyjscie_obiektu; }
    float odczytaj_sterowanie() const { return s_sterowanie; }

    void wypisz_parametry();
    std::map<std::string, float> get_s_parametry() const;
protected:
    float s_wartosc_zadana;
    float s_uchyb;
    float s_wyjscie_obiektu;
    float s_sterowanie;
    std::unique_ptr<Sygnal> s_generator;

    Odczyt_xml *s_czytnik_xml;

    std::string s_typ;

    std::map< std::string, float > s_parametry;
};

#endif // REGULATOR_H
