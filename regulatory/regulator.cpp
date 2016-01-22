#include "regulator.h"
#include "odczyt_xml.h"
#include <iterator>
#include <iostream>

/**
 * @brief Regulator::Regulator - konstruktor
 * @param x - uchwyt do pliku XML
 * @param typ - nazwa typu regulator (np. "PID")
 */
Regulator::Regulator(Odczyt_xml *x, std::string typ): s_czytnik_xml(x), s_typ(typ)
{
    ustaw_parametry();
}

Regulator::Regulator(std::string typ): s_typ(typ)
{
    s_wartosc_zadana = 0;
    s_uchyb = 0;
    s_wyjscie_obiektu = 0;
    s_sterowanie = 0;
}

Regulator::Regulator(std::map<std::string, float> parametry, std::string typ): s_typ(typ), s_parametry(parametry)
{
    s_wartosc_zadana = 0;
    s_uchyb = 0;
    s_wyjscie_obiektu = 0;
    s_sterowanie = 0;
}

/**
 * @brief Regulator::~Regulator
 */
Regulator::~Regulator()
{

}

/**
 * @brief Regulator::symuluj - oblicza sterowanie na podstawie wyjścia obiektu,
 *          wartości zadanej i struktury regulatora
 * @param u - wejście do sterownika (wyjśćie obiektu)
 * @return wartość sterowania
 */
float Regulator::symuluj(float u){
    std::cout << "regulator.symuluj" << std::endl;
    s_wyjscie_obiektu = u;
    s_uchyb = s_wartosc_zadana - s_wyjscie_obiektu;
    s_sterowanie = s_uchyb;
    return s_sterowanie;
}

/**
 * @brief Regulator::ustaw_parametry -
 *          pobiera z pliku XML dane zgodne z typem regulatora (s_typ)
 */
void Regulator::ustaw_parametry(){
    if(s_czytnik_xml->otworz_plik_xml()){
        tinyxml2::XMLElement *poziom = s_czytnik_xml->xmlDoc.FirstChildElement("Regulatory")->FirstChildElement("Parametry");

        for( tinyxml2::XMLElement *regulator = poziom->FirstChildElement(); regulator != NULL; regulator = regulator->NextSiblingElement()){
            if( s_typ.compare(regulator->FirstAttribute()->Value()) == 0){
                const tinyxml2::XMLAttribute *atrybut = regulator->FirstAttribute();
                atrybut = atrybut->Next();
                while (atrybut) {
                    s_parametry[atrybut->Name()] = atrybut->DoubleValue();
                    atrybut = atrybut->Next();
                }
            }
        }
    }
}

void Regulator::ustaw_parametry(std::map<std::string, float> parametry)
{
    s_parametry = parametry;
}

/**
 * @brief Regulator::zapisz_parametry -
 *          zapisuje dane do pliku XML zgodnie z typem regulatora
 */
void Regulator::zapisz_parametry(){
    if(s_czytnik_xml->otworz_plik_xml()){
        std::map< std::string, float >::iterator iter;
        tinyxml2::XMLElement *poziom = s_czytnik_xml->xmlDoc.FirstChildElement("Regulatory")->FirstChildElement("Parametry");

        for( tinyxml2::XMLElement *regulator = poziom->FirstChildElement(); regulator != NULL; regulator = regulator->NextSiblingElement()){
            if( s_typ.compare(regulator->FirstAttribute()->Value()) == 0 ){
                for(iter = s_parametry.begin(); iter != s_parametry.end(); ++iter){
                    regulator->SetAttribute(iter->first.c_str(), iter->second);
                    s_czytnik_xml->xmlDoc.SaveFile("appConfig.xml");
                }
            }
        }
    }
}

/**
 * @brief Regulator::wypisz_parametry -
 *          pomocnicza funkcja
 */
void Regulator::wypisz_parametry() {
    std::map< std::string, float >::iterator iter;
    for(iter = s_parametry.begin(); iter != s_parametry.end(); ++iter){

        std::cout << iter->first << "  " << iter->second << std::endl;
    }
}

std::map<std::string, float> Regulator::get_s_parametry() const
{
    return s_parametry;
}
