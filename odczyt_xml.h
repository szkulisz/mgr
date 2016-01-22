#ifndef ODCZYT_XML_H
#define ODCZYT_XML_H

#include "tinyxml2.h"
#include <vector>
#include <string>
#include <map>

class Odczyt_xml
{
public:
    Odczyt_xml();
    ~Odczyt_xml();

    std::map< std::string, std::string > odczytaj_konfiguracje_obiektu();
    std::string odczytaj_typ_regulatora();

    bool otworz_plik_xml();

//private:
    tinyxml2::XMLDocument xmlDoc;

};

#endif // ODCZYT_XML_H
