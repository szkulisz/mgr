#include "regulatorpid.h"
#include <map>
#include <iterator>
#include <iostream>

#include "regulatory/dekorator_sygnalu.h"


RegulatorPID::RegulatorPID(Odczyt_xml *x ) : Regulator( x, "PID" )
{
    try{
        ustaw_parametry_PID();
    }
    catch (std::string & ){
        throw;
    }

}

RegulatorPID::RegulatorPID(std::map<std::string, float> parametry) : Regulator( parametry, "PID" )
{
    try{
        ustaw_parametry_PID();
    }
    catch (std::string & ){
        throw;
    }
}

RegulatorPID::~RegulatorPID()
{

}
/*!
 * \brief RegulatorPID::ustaw_parametry
 * Funkcja ustawia parametry regulatora PID.
 */
void RegulatorPID::ustaw_parametry(){
    Regulator::ustaw_parametry();
    try{

        ustaw_parametry_PID();
    }
    catch (std::string & ){
        throw;
    }
}

void RegulatorPID::ustaw_parametry(std::map<std::string, float> parametry)
{
    Regulator::ustaw_parametry(parametry);
    try{

        ustaw_parametry_PID();
    }
    catch (std::string & ){
        throw;
    }

}
/*!
 * \brief RegulatorPID::ustaw_parametry_PID
 * Funckja pomocnicza przypisująca parametry z mapy do zmiennych.
 */
void RegulatorPID::ustaw_parametry_PID(){
    try{
        sprawdz_parametry();
        s_k = s_parametry["k"];
        s_Td = s_parametry["Td"];
        s_N = s_parametry["N"];
        s_Tp = s_parametry["Tp"];
        s_b = s_parametry["b"];
        s_Ti = s_parametry["Ti"];
        s_max = s_parametry["max"];
        s_min = s_parametry["min"];
//        if (s_parametry["ogr"] != 0)
            s_ograniczone = s_parametry["ogr"];//true;
    }
    catch (std::string & e){
        std::cout << e << std::endl;
        throw;
    }
}
/*!
 * \brief RegulatorPID::sprawdz_parametry
 * Sprawdza wartości parametrów.
 */
void RegulatorPID::sprawdz_parametry(){
    for(std::map<std::string,float>::iterator it = s_parametry.begin(); it != s_parametry.end(); ++it){
        if(it->second < 0 && it->first != "min")
            throw std::string("Parametry musza byc dodatnie");
    }

    float td = s_parametry["Td"];
    float n = s_parametry["N"];
    float tp = s_parametry["Tp"];
    if(td <= (n * tp)/2)
        throw std::string("Blad modulu");
}
/*!
 * \brief RegulatorPID::proportional
 * Implementacja części proporcjonalnej regulatora
 * \param w wartośc zadana
 * \param y poprzednie wyjście obiektu
 * \return Wartość sterowania części proporcjonalnej
 */
float RegulatorPID::proportional(float w, float y){
    return s_k * (s_b * w - y);
}
/*!
 * \brief RegulatorPID::integral
 * Implementacja części całkującej regulatora PID.
 * \param e uchyb
 * \return Wartość sterowania części całkującej
 */
float RegulatorPID::integral(float e)
{
    return s_I_poprzedni + ( (s_k * s_Tp * e) / s_Ti );
}
/*!
 * \brief RegulatorPID::differential
 * Implementacja części różniczkującej regulatora PID.
 * \param y poprzednie wyjście obiektu (i-1)
 * \param y_poprzedni wyjście obiektu z chwili i-2
 * \return Wartość sterowania części różniczkującej
 */
float RegulatorPID::differential(float y, float y_poprzedni)
{
    return ( s_Td / (s_Td + s_N * s_Tp) ) * s_D_poprzedni - s_k * ( (s_N * s_Td) / (s_Td + s_N * s_Tp) ) * (y - y_poprzedni);
}
/*!
 * \brief RegulatorPID::symuluj
 * Funkcja pętli symulacji obiektu PID
 * \param y wyjście obiektu z chwili poprzedniej
 * \return Sterowanie
 */
float RegulatorPID::symuluj(float y){
    if(s_generator){
        s_wartosc_zadana = s_generator->generuj();
    }
        s_y_poprzednie = s_wyjscie_obiektu;
        s_wyjscie_obiektu = y;

        s_uchyb = s_wartosc_zadana - s_wyjscie_obiektu;

        const float p = proportional(s_wartosc_zadana, y);
        const float i = integral(s_uchyb);
        const float d = differential(y,s_y_poprzednie);
        s_sterowanie = i + p + d;
        s_D_poprzedni = d;

        // jeśli sterowanie jest ograniczone
        // i przekroczyłeś granice,
        // to anti-windup i ogranicz sterowanie
        if ( s_ograniczone ) {
            if ( s_sterowanie > s_max ) {
                s_sterowanie = s_max;
                return s_sterowanie;
            }
            if ( s_sterowanie < s_min ){
                s_sterowanie = s_min;
                return s_sterowanie;
            }
        }

        s_I_poprzedni = i;

        return s_sterowanie;

}
