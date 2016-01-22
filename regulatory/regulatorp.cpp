#include "regulatorp.h"
#include "odczyt_xml.h"
#include "regulatory/dekorator_sygnalu.h"
#include <iostream>

/**
 * @brief RegulatorP::RegulatorP -
 *          konstruktor regulatora typu P
 * @param x - uchwyt do pliku XML
 */
RegulatorP::RegulatorP( Odczyt_xml *x ) : Regulator( x, "P" )
{
}

RegulatorP::RegulatorP(std::map<std::string, float> parametry) : Regulator(parametry, "P")
{

}

/**
 * @brief RegulatorP::~RegulatorP
 */
RegulatorP::~RegulatorP()
{

}

/**
 * @brief RegulatorP::symuluj - implementuje działanie regulatora.
 *          zawiera również generowanie wartości zadanej
 * @param u - wejście do regulatora
 * @return wypracowane sterowanie
 */
float RegulatorP::symuluj(float u){
    //std::cout << "regulatorP.symuluj" << std::endl;
    s_wyjscie_obiektu = u;
    s_wartosc_zadana = s_generator->generuj();
    s_uchyb = s_wartosc_zadana - s_wyjscie_obiektu;
    s_sterowanie = s_parametry.begin()->second * s_uchyb;
    s_sterowanie=s_wartosc_zadana;
    return s_sterowanie;
}

/**
 * @brief RegulatorP::ustaw_wzmocnienie - pozwala na ustawienie wzmocnienia
 * @param k - nowa wartość wzmocnienia
 */
void RegulatorP::ustaw_wzmocnienie(float k){
    s_parametry["k"]=k;
}

